/* ============================================================
   SVG 交互图引擎 —— 手写，无依赖，断网可用。

   它做什么：
     页面里放一个空的 <div class="geo" data-geo="键"></div>，
     页面末尾用 GEO.键 = { ... } 给出配置；
     这个脚本把它画成一张 SVG 图，下面带滑块、步进按钮和一行读数。
     配置里的 draw(g, s, step) 负责画什么，引擎负责坐标换算、
     控件、读数、出错提示和亮暗配色。

   它不做什么：
     它不做数值求解。图里的数都由页面的 draw 现算，
     算错了检查脚本会看出来；引擎不替页面猜。

   配置字段：
     title      SVG 的标题（读屏软件用）
     desc       图的说明（可选）
     w, h       viewBox 的宽高，默认 640 × 400
     xr, yr     数学坐标范围，例如 [-4, 4]、[-3, 3]
     pad        数学坐标四周留白，默认 34；示意图可写 0
     aspect     'equal' 等比（几何图）或 'fit' 各自铺满（函数图）
     yp         'up' 数学 y 向上（默认）或 'down' 屏幕 y 向下
     controls   [{ id, label, min, max, step, value, unit }]
     actions    [{ label, run(g, s) }]
     steps      步数，或每一步的说明数组
     draw       function (g, s, step) { ... }

   脚本顺序：toc.js → code.js → geo.js → 页面的 GEO.* 配置。
   ============================================================ */

(function (global) {
  global.GEO = global.GEO || {};
  "use strict";

  var NS = "http://www.w3.org/2000/svg";
  var seq = 0;

  function el(tag, cls) {
    var e = document.createElement(tag);
    if (cls) e.className = cls;
    return e;
  }

  function svgEl(tag, cls) {
    var e = document.createElementNS(NS, tag);
    if (cls) e.setAttribute("class", cls);
    return e;
  }

  function clamp(v, lo, hi) {
    return v < lo ? lo : (v > hi ? hi : v);
  }

  function fmt(v, n) {
    if (!isFinite(v)) return "—";
    if (Math.abs(v) < 0.0005) v = 0;
    return v.toFixed(n);
  }

  function decimals(step) {
    if (!(step > 0) || step >= 1) return 0;
    return Math.min(3, Math.max(0, Math.ceil(-Math.log(step) / Math.LN10)));
  }

  // ----------------------------------------------------------
  // 一张图
  // ----------------------------------------------------------

  function Figure(root, key, cfg) {
    var n = ++seq;
    this.root = root;
    this.key = key;
    this.cfg = cfg;
    this.idbase = "geo-" + n;
    this.state = {};
    this.step = 0;
    this.controls = {};
    this.errors = [];
    this.stepNotes = Object.prototype.toString.call(cfg.steps) === "[object Array]"
      ? cfg.steps : null;
    this.nsteps = this.stepNotes ? this.stepNotes.length : (cfg.steps || 0);
    this.build();
    this.resetState();
    if (this.nsteps > 0) this.go(0);
    else this.render();
  }

  Figure.prototype.build = function () {
    var self = this;
    var cfg = this.cfg;
    var i;

    while (this.root.firstChild) this.root.removeChild(this.root.firstChild);
    this.root.setAttribute("data-geo-ready", "1");
    this.root.setAttribute("role", "group");
    this.root.setAttribute("aria-label", cfg.title || this.key);

    this.W = cfg.w || 640;
    this.H = cfg.h || 400;
    var xr = cfg.xr || [0, this.W];
    var yr = cfg.yr || [0, this.H];
    var pad = cfg.pad === undefined ? (cfg.xr ? 34 : 0) : cfg.pad;
    var innerW = Math.max(1, this.W - 2 * pad);
    var innerH = Math.max(1, this.H - 2 * pad);
    var sx = innerW / (xr[1] - xr[0]);
    var sy = innerH / (yr[1] - yr[0]);
    if (cfg.aspect === "equal") {
      var s = Math.min(sx, sy);
      this.ox = pad + (innerW - (xr[1] - xr[0]) * s) / 2;
      this.oy = pad + (innerH - (yr[1] - yr[0]) * s) / 2;
      this.sx = s;
      this.sy = s;
    } else {
      this.ox = pad;
      this.oy = pad;
      this.sx = sx;
      this.sy = sy;
    }
    this.x0 = xr[0];
    this.x1 = xr[1];
    this.y0 = yr[0];
    this.y1 = yr[1];
    this.yp = cfg.yp === "down" ? "down" : "up";

    // ---- SVG ----
    this.svg = svgEl("svg", "geo-svg");
    this.svg.setAttribute("viewBox", "0 0 " + this.W + " " + this.H);
    this.svg.setAttribute("role", "img");
    this.svg.setAttribute("preserveAspectRatio", "xMidYMid meet");
    if (cfg.title) {
      var t = svgEl("title");
      t.textContent = cfg.title;
      this.svg.appendChild(t);
    }
    if (cfg.desc) {
      var d = svgEl("desc");
      d.textContent = cfg.desc;
      this.svg.appendChild(d);
    }
    this.layer = svgEl("g");
    this.svg.appendChild(this.layer);
    this.root.appendChild(this.svg);

    // ---- 控件与读数 ----
    this.panel = el("div", "geo-panel");
    this.root.appendChild(this.panel);

    if (cfg.controls && cfg.controls.length) {
      var ctrl = el("div", "geo-ctrl");
      for (i = 0; i < cfg.controls.length; i++) {
        this.addControl(ctrl, cfg.controls[i]);
      }
      this.panel.appendChild(ctrl);
    }

    var stepsBox = el("div", "geo-steps");
    if (this.nsteps > 0) this.addSteps(stepsBox);
    if (cfg.actions && cfg.actions.length) {
      for (i = 0; i < cfg.actions.length; i++) {
        this.addAction(stepsBox, cfg.actions[i], i);
      }
    }
    if (stepsBox.firstChild) this.panel.appendChild(stepsBox);

    this.readoutEl = el("div", "geo-readout");
    this.panel.appendChild(this.readoutEl);

    // ---- draw 用的门面 ----
    this.g = {
      W: this.W,
      H: this.H,
      x: function (v) { return self.px(v); },
      y: function (v) { return self.py(v); },
      grid: function (a, b) { return self.grid(a, b); },
      axes: function (o) { return self.axes(o); },
      line: function () { return self.line.apply(self, arguments); },
      arrow: function () { return self.arrow.apply(self, arguments); },
      poly: function () { return self.poly.apply(self, arguments); },
      circle: function () { return self.circle.apply(self, arguments); },
      dot: function () { return self.dot.apply(self, arguments); },
      ellipse: function () { return self.ellipse.apply(self, arguments); },
      text: function () { return self.text.apply(self, arguments); },
      rect: function () { return self.rect.apply(self, arguments); },
      readout: function (s) { return self.readout(s); },
      fmt: fmt,
      set: function (id, v) { return self.set(id, v); },
      redraw: function () { return self.render(); }
    };
  };

  Figure.prototype.addControl = function (box, c) {
    var self = this;
    var id = this.idbase + "-" + c.id;
    var wrap = el("div", "geo-ctl");
    var label = el("label", "geo-ctl-label");
    var input = document.createElement("input");
    var out = el("output", "geo-ctl-val");
    var value = c.value !== undefined ? c.value : c.min;

    label.textContent = c.label;
    label.setAttribute("for", id);

    input.type = "range";
    input.id = id;
    input.className = "geo-range";
    input.min = c.min;
    input.max = c.max;
    input.step = c.step;
    input.value = value;
    input.setAttribute("data-ctl", c.id);
    input.setAttribute("aria-label", c.label);

    out.setAttribute("for", id);
    out.textContent = this.fmtControl(c, value);

    input.addEventListener("input", function () {
      var v = parseFloat(input.value);
      self.state[c.id] = v;
      out.textContent = self.fmtControl(c, v);
      self.render();
    });

    wrap.appendChild(label);
    wrap.appendChild(input);
    wrap.appendChild(out);
    box.appendChild(wrap);
    this.controls[c.id] = { input: input, out: out, cfg: c };
    this.state[c.id] = value;
  };

  Figure.prototype.addSteps = function (box) {
    var self = this;
    var prev = el("button", "viz-btn");
    var next = el("button", "viz-btn");
    var reset = el("button", "viz-btn viz-btn-ghost");
    var count = el("span", "viz-count");

    prev.type = next.type = reset.type = "button";
    prev.textContent = "← 上一步";
    next.textContent = "下一步 →";
    reset.textContent = "↺ 重来";
    prev.setAttribute("data-geo-act", "prev");
    next.setAttribute("data-geo-act", "next");
    reset.setAttribute("data-geo-act", "reset");

    prev.addEventListener("click", function () { self.go(self.step - 1); });
    next.addEventListener("click", function () { self.go(self.step + 1); });
    reset.addEventListener("click", function () { self.reset(); });

    box.appendChild(prev);
    box.appendChild(next);
    box.appendChild(reset);
    box.appendChild(count);
    this.prevBtn = prev;
    this.nextBtn = next;
    this.countEl = count;
  };

  Figure.prototype.addAction = function (box, action, i) {
    var self = this;
    var btn = el("button", "viz-btn" + (action.ghost ? " viz-btn-ghost" : ""));
    btn.type = "button";
    btn.textContent = action.label;
    btn.setAttribute("data-geo-action", String(i));
    btn.addEventListener("click", function () {
      try {
        action.run(self.g, self.state);
      } catch (e) {
        self.fail(e);
        return;
      }
      self.render();
    });
    box.appendChild(btn);
  };

  Figure.prototype.fmtControl = function (c, v) {
    return v.toFixed(decimals(c.step)) + (c.unit || "");
  };

  Figure.prototype.resetState = function () {
    var k;
    for (k in this.controls) {
      if (Object.prototype.hasOwnProperty.call(this.controls, k)) {
        this.controls[k].input.value = this.state[k];
        this.controls[k].out.textContent =
          this.fmtControl(this.controls[k].cfg, this.state[k]);
      }
    }
  };

  Figure.prototype.set = function (id, v) {
    var ctl = this.controls[id];
    if (!ctl) return;
    this.state[id] = v;
    ctl.input.value = v;
    ctl.out.textContent = this.fmtControl(ctl.cfg, v);
  };

  Figure.prototype.reset = function () {
    this.go(0);
  };

  Figure.prototype.go = function (n) {
    if (this.nsteps <= 0) return;
    n = clamp(n, 0, this.nsteps - 1);
    this.step = n;
    this.state.step = n;
    if (this.prevBtn) this.prevBtn.disabled = n === 0;
    if (this.nextBtn) this.nextBtn.disabled = n === this.nsteps - 1;
    if (this.countEl) {
      this.countEl.textContent = "第 " + (n + 1) + " / " + this.nsteps + " 步";
    }
    this.render();
  };

  // ----------------------------------------------------------
  // 坐标与图元
  // ----------------------------------------------------------

  Figure.prototype.px = function (x) {
    return this.ox + (x - this.x0) * this.sx;
  };

  Figure.prototype.py = function (y) {
    if (this.yp === "down") return this.oy + (y - this.y0) * this.sy;
    return this.oy + (this.y1 - y) * this.sy;
  };

  Figure.prototype.grid = function (stepX, stepY) {
    if (!this.cfg.xr || !this.cfg.yr) return;
    stepX = stepX || 1;
    stepY = stepY || 1;
    var spanX = this.x1 - this.x0;
    var spanY = this.y1 - this.y0;
    if (spanX / stepX > 40) stepX = spanX / 40;
    if (spanY / stepY > 40) stepY = spanY / 40;
    var x, y;
    for (x = Math.ceil(this.x0 / stepX) * stepX; x <= this.x1 + 1e-9; x += stepX) {
      if (Math.abs(x) < 1e-9) continue;
      this.line(x, this.y0, x, this.y1, "g-grid");
    }
    for (y = Math.ceil(this.y0 / stepY) * stepY; y <= this.y1 + 1e-9; y += stepY) {
      if (Math.abs(y) < 1e-9) continue;
      this.line(this.x0, y, this.x1, y, "g-grid");
    }
  };

  Figure.prototype.axes = function (opts) {
    if (!this.cfg.xr || !this.cfg.yr) return;
    opts = opts || {};
    var yAxis = (this.y0 <= 0 && this.y1 >= 0) ? 0 : this.y0;
    var xAxis = (this.x0 <= 0 && this.x1 >= 0) ? 0 : this.x0;
    this.arrow(this.x0, yAxis, this.x1, yAxis, "g-axis");
    this.arrow(xAxis, this.y0, xAxis, this.y1, "g-axis");
    if (opts.labels !== false) {
      this.text(this.x1, yAxis, "x", "g-label", "end", -11);
      this.text(xAxis, this.y1, "y", "g-label", "end", -11);
    }
  };

  Figure.prototype.line = function (x1, y1, x2, y2, cls, dash) {
    var e = svgEl("line", cls || "g-line");
    e.setAttribute("x1", this.px(x1));
    e.setAttribute("y1", this.py(y1));
    e.setAttribute("x2", this.px(x2));
    e.setAttribute("y2", this.py(y2));
    if (dash) e.setAttribute("stroke-dasharray", "6 5");
    this.layer.appendChild(e);
    return e;
  };

  Figure.prototype.arrow = function (x1, y1, x2, y2, cls) {
    var p1x = this.px(x1), p1y = this.py(y1);
    var p2x = this.px(x2), p2y = this.py(y2);
    var dx = p2x - p1x, dy = p2y - p1y;
    var len = Math.sqrt(dx * dx + dy * dy);
    if (len < 1.5) return;
    var ux = dx / len, uy = dy / len;
    var head = Math.min(10, len * 0.5);
    var half = head * 0.45;
    var bx = p2x - ux * head, by = p2y - uy * head;

    var line = svgEl("line", (cls || "g-c1") + " g-line");
    line.setAttribute("x1", p1x);
    line.setAttribute("y1", p1y);
    line.setAttribute("x2", bx);
    line.setAttribute("y2", by);
    this.layer.appendChild(line);

    var poly = svgEl("polygon", (cls || "g-c1") + " g-head");
    poly.setAttribute("points",
      p2x + "," + p2y + " " +
      (bx - uy * half) + "," + (by + ux * half) + " " +
      (bx + uy * half) + "," + (by - ux * half));
    this.layer.appendChild(poly);
  };

  Figure.prototype.poly = function (points, cls, closed) {
    var e = svgEl(closed ? "polygon" : "polyline", cls || "g-line");
    var parts = [];
    for (var i = 0; i < points.length; i++) {
      parts.push(this.px(points[i][0]) + "," + this.py(points[i][1]));
    }
    e.setAttribute("points", parts.join(" "));
    this.layer.appendChild(e);
    return e;
  };

  Figure.prototype.circle = function (cx, cy, r, cls) {
    var e = svgEl("circle", cls || "g-c1");
    e.setAttribute("cx", this.px(cx));
    e.setAttribute("cy", this.py(cy));
    e.setAttribute("r", Math.max(0, r * (this.sx + this.sy) / 2));
    this.layer.appendChild(e);
    return e;
  };

  Figure.prototype.dot = function (x, y, cls) {
    var e = svgEl("circle", (cls || "g-c1") + " g-dot");
    e.setAttribute("cx", this.px(x));
    e.setAttribute("cy", this.py(y));
    e.setAttribute("r", 4);
    this.layer.appendChild(e);
    return e;
  };

  Figure.prototype.ellipse = function (cx, cy, rx, ry, rot, cls) {
    var pts = [];
    var n = 96;
    var c = Math.cos(rot || 0), s = Math.sin(rot || 0);
    for (var i = 0; i < n; i++) {
      var t = i * 2 * Math.PI / n;
      var xe = rx * Math.cos(t), ye = ry * Math.sin(t);
      pts.push([cx + xe * c - ye * s, cy + xe * s + ye * c]);
    }
    return this.poly(pts, cls || "g-c3", true);
  };

  Figure.prototype.text = function (x, y, str, cls, anchor, dy) {
    var e = svgEl("text", "g-label " + (cls || ""));
    e.setAttribute("x", this.px(x));
    e.setAttribute("y", this.py(y) + (dy || 0));
    e.setAttribute("text-anchor", anchor || "middle");
    e.setAttribute("dominant-baseline", "middle");
    e.textContent = str;
    this.layer.appendChild(e);
    return e;
  };

  Figure.prototype.rect = function (x, y, w, h, cls) {
    var e = svgEl("rect", cls || "g-grid");
    var py = this.py(y);
    if (this.yp === "up") py = this.py(y + h);
    e.setAttribute("x", this.px(x));
    e.setAttribute("y", py);
    e.setAttribute("width", Math.abs(w * this.sx));
    e.setAttribute("height", Math.abs(h * this.sy));
    this.layer.appendChild(e);
    return e;
  };

  Figure.prototype.readout = function (s) {
    if (this.readoutEl) this.readoutEl.textContent = s;
  };

  // ----------------------------------------------------------
  // 重画与出错
  // ----------------------------------------------------------

  Figure.prototype.render = function () {
    var i;
    while (this.layer.firstChild) this.layer.removeChild(this.layer.firstChild);
    if (this.errEl && this.errEl.parentNode) {
      this.errEl.parentNode.removeChild(this.errEl);
    }
    this.errEl = null;
    this.root.removeAttribute("data-geo-error");
    this.readoutEl.textContent = "";
    this.state.step = this.step;
    try {
      this.cfg.draw(this.g, this.state, this.step);
    } catch (e) {
      this.fail(e);
    }
  };

  Figure.prototype.fail = function (e) {
    var msg = e && e.message ? e.message : String(e);
    this.errors.push(msg);
    if (global.GEO.errors) {
      global.GEO.errors.push({ key: this.key, message: msg });
    }
    while (this.layer.firstChild) this.layer.removeChild(this.layer.firstChild);
    this.errEl = el("div", "geo-error");
    this.errEl.textContent = "这张图没有画出来：" + msg;
    this.panel.appendChild(this.errEl);
    this.root.setAttribute("data-geo-error", "1");
    if (global.console && global.console.error) {
      global.console.error("[geo] " + this.key + "：" + msg);
    }
  };

  // ----------------------------------------------------------
  // 挂载
  // ----------------------------------------------------------

  global.GEO.errors = global.GEO.errors || [];

  function mountAll() {
    var nodes = document.querySelectorAll("[data-geo]");
    var list = global.__GEO_FIGURES || (global.__GEO_FIGURES = []);
    for (var i = 0; i < nodes.length; i++) {
      var node = nodes[i];
      if (node.getAttribute("data-geo-ready") === "1") continue;
      var key = node.getAttribute("data-geo");
      var cfg = global.GEO[key];
      if (!cfg || typeof cfg.draw !== "function") {
        while (node.firstChild) node.removeChild(node.firstChild);
        var miss = el("div", "geo-missing");
        miss.textContent = "这张图没有配置（" + key + "）。";
        node.appendChild(miss);
        node.setAttribute("data-geo-ready", "1");
        node.setAttribute("data-geo-missing", "1");
        continue;
      }
      list.push(new Figure(node, key, cfg));
    }
  }

  global.GEO.mountAll = mountAll;
  global.GEO.Figure = Figure;

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", mountAll);
  } else {
    mountAll();
  }
})(window);
