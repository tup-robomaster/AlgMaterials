/* ============================================================
   数据流动画播放器 —— 手写，无依赖，断网可用。

   它做什么：
     上面一行代码，当前执行到的那行高亮；
     中间左边是内存（一排盒子），右边是屏幕（已经打出来的字）；
     数据搬运的时候，从哪搬到哪画一条箭头；
     底下「上一步 / 下一步 / 自动播放」。

   它不做什么：
     它不解析 C++。每一步的状态是**手写**在课里的。
     所以它绝不会自己算出一个错的结果——但也因此，只有课里挑出来的
     那几处用得上它（目录页上标着「★ 有动画」的那些节）。

   数据格式（写在页面里）：

     VIZ.boxes = {
       code: [
         "int a = 3;",
         "int b = 5;",
         "int c = a + b;"
       ],
       steps: [
         {
           line: 0,                    // 高亮第几行（从 0 数）；null = 不高亮
           note: "开一个盒子叫 a，把 3 放进去。",
           mem: [                      // 这一步结束时，内存里有什么
             { name: "a", value: 3, state: "new" }
           ],
           screen: [],                 // 屏幕上已经打出来的行
           flows: []                   // 搬运箭头：[{from:"a", to:"c"}]
         }
       ]
     };

     state 可选： "new"（刚建出来） "read"（这一步被读走）
                 "write"（这一步被写入）
                 "gone"（这一步之后就没了，画成灰的；下一步从 mem 里拿掉）
                 "" （没事发生）
   ============================================================ */

(function (global) {

  // 命名空间。讲义页里写 VIZ.boxes = {...} 来注册动画配置，
  // 这里必须先把它建出来：页面里的脚本一旦带 "use strict"，
  // 「给一个没声明过的变量赋值」会直接抛 ReferenceError，
  // 页面看起来就是「动画没了」，而且什么都不提示。
  global.VIZ = global.VIZ || {};
  "use strict";

  function esc(s) {
    return String(s)
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;");
  }

  function el(tag, cls) {
    var e = document.createElement(tag);
    if (cls) e.className = cls;
    return e;
  }

  // ----------------------------------------------------------
  // 一个播放器实例
  // ----------------------------------------------------------

  function Player(root, cfg) {
    this.root = root;
    this.cfg = cfg;
    this.steps = cfg.steps || [];
    this.i = 0;
    this.timer = null;
    this.prevMem = null;      // 上一步的内存状态，用来判断哪个盒子变了
    this.build();
    this.go(0);
    this.bind();
  }

  Player.prototype.build = function () {
    var cfg = this.cfg;
    var k;

    root_clear(this.root);

    // ---- 代码 ----
    var code = el("div", "viz-code");
    this.lineEls = [];
    for (k = 0; k < cfg.code.length; k++) {
      var ln = el("div", "ln");
      var no = el("span", "ln-no");
      var src = el("span", "ln-src");
      no.textContent = k + 1;
      src.innerHTML = esc(cfg.code[k]) || "&nbsp;";
      ln.appendChild(no);
      ln.appendChild(src);
      code.appendChild(ln);
      this.lineEls.push(ln);
    }
    this.root.appendChild(code);

    // ---- 内存 + 屏幕 ----
    var panels = el("div", "viz-panels");

    var stage = el("div", "viz-stage");
    this.mem = el("div", "viz-mem");
    stage.appendChild(this.mem);
    this.stage = stage;
    panels.appendChild(stage);

    var screenBox = el("div", "viz-screen");
    var sh = el("div", "viz-screen-h");
    sh.textContent = "屏幕";
    this.screenBody = el("div", "viz-screen-b");
    screenBox.appendChild(sh);
    screenBox.appendChild(this.screenBody);
    panels.appendChild(screenBox);

    // 箭头要跨「内存」和「屏幕」两块，所以 SVG 盖在整个 panels 上
    this.svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    this.svg.setAttribute("class", "viz-svg");
    panels.appendChild(this.svg);
    this.panels = panels;

    this.root.appendChild(panels);

    // ---- 说明 ----
    this.noteEl = el("div", "viz-note");
    this.root.appendChild(this.noteEl);

    // ---- 控制条 ----
    var ctrl = el("div", "viz-ctrl");

    this.btnPrev = el("button", "viz-btn");
    this.btnPrev.type = "button";
    this.btnPrev.textContent = "← 上一步";

    this.btnNext = el("button", "viz-btn");
    this.btnNext.type = "button";
    this.btnNext.textContent = "下一步 →";

    this.btnPlay = el("button", "viz-btn viz-btn-play");
    this.btnPlay.type = "button";
    this.btnPlay.textContent = "▶ 自动播放";

    this.btnReset = el("button", "viz-btn viz-btn-ghost");
    this.btnReset.type = "button";
    this.btnReset.textContent = "↺ 重来";

    this.countEl = el("span", "viz-count");

    ctrl.appendChild(this.btnPrev);
    ctrl.appendChild(this.btnNext);
    ctrl.appendChild(this.btnPlay);
    ctrl.appendChild(this.btnReset);
    ctrl.appendChild(this.countEl);
    this.root.appendChild(ctrl);

    // 键盘左右键也能翻——讲课时用
    this.root.setAttribute("tabindex", "0");
  };

  Player.prototype.bind = function () {
    var self = this;

    this.btnPrev.addEventListener("click", function () { self.stop(); self.go(self.i - 1); });
    this.btnNext.addEventListener("click", function () { self.stop(); self.go(self.i + 1); });
    this.btnReset.addEventListener("click", function () { self.stop(); self.go(0); });

    this.btnPlay.addEventListener("click", function () {
      if (self.timer) { self.stop(); return; }
      if (self.i >= self.steps.length - 1) self.go(0);
      self.btnPlay.textContent = "❚❚ 暂停";
      self.btnPlay.classList.add("on");
      self.timer = setInterval(function () {
        if (self.i >= self.steps.length - 1) { self.stop(); return; }
        self.go(self.i + 1);
      }, 1500);
    });

    this.root.addEventListener("keydown", function (ev) {
      if (ev.key === "ArrowLeft")  { self.stop(); self.go(self.i - 1); ev.preventDefault(); }
      if (ev.key === "ArrowRight") { self.stop(); self.go(self.i + 1); ev.preventDefault(); }
    });

    // 窗口大小变了，箭头位置要重算
    var onResize = function () { self.drawFlows(self.steps[self.i]); };
    global.addEventListener("resize", onResize);
    this._onResize = onResize;
  };

  Player.prototype.stop = function () {
    if (this.timer) { clearInterval(this.timer); this.timer = null; }
    this.btnPlay.textContent = "▶ 自动播放";
    this.btnPlay.classList.remove("on");
  };

  // ----------------------------------------------------------
  // 跳到第 n 步
  // ----------------------------------------------------------

  Player.prototype.go = function (n) {
    if (n < 0) n = 0;
    if (n > this.steps.length - 1) n = this.steps.length - 1;
    this.i = n;

    var st = this.steps[n];
    var k;

    // 代码高亮
    for (k = 0; k < this.lineEls.length; k++) {
      this.lineEls[k].classList.toggle("on", k === st.line);
    }

    // 内存
    this.mem.innerHTML = "";
    var mem = st.mem || [];
    var prev = this.prevMem || [];
    var prevOf = {};
    for (k = 0; k < prev.length; k++) prevOf[prev[k].name] = prev[k].value;

    for (k = 0; k < mem.length; k++) {
      var box = el("div", "viz-box");
      var m = mem[k];
      box.setAttribute("data-name", m.name);

      var nameEl = el("div", "viz-box-name");
      nameEl.textContent = m.name;
      var valEl = el("div", "viz-box-val");

      // 值刚变了吗？变了就做一下动效，让人看得见
      var changed = prevOf.hasOwnProperty(m.name) && prevOf[m.name] !== m.value;
      var isNew = !prevOf.hasOwnProperty(m.name);
      if (changed) {
        valEl.classList.add("bump");
        box.classList.add("changed");
      }
      if (isNew && n > 0) box.classList.add("appear");

      valEl.textContent = m.value;
      box.appendChild(nameEl);
      box.appendChild(valEl);

      if (m.state) box.classList.add("is-" + m.state);
      this.mem.appendChild(box);
    }

    // 屏幕上打出来的东西
    var scr = st.screen || [];
    this.screenBody.innerHTML = "";
    if (!scr.length) {
      var empty = el("div", "viz-screen-empty");
      empty.textContent = "（还没有输出）";
      this.screenBody.appendChild(empty);
    } else {
      for (k = 0; k < scr.length; k++) {
        var row = el("div", "viz-screen-line");
        row.textContent = scr[k];
        if (k === scr.length - 1 && (st.note || "").indexOf("屏幕") >= 0) {
          row.classList.add("bump");
        }
        this.screenBody.appendChild(row);
      }
    }

    // 说明
    this.noteEl.innerHTML = st.note || "";

    // 按钮状态
    this.btnPrev.disabled = n === 0;
    this.btnNext.disabled = n === this.steps.length - 1;
    this.countEl.textContent = "第 " + (n + 1) + " / " + this.steps.length + " 步";

    // 箭头（要等布局完成才能量位置）
    var self = this;
    global.requestAnimationFrame(function () { self.drawFlows(st); });

    this.prevMem = mem;
  };

  // ----------------------------------------------------------
  // 画搬运箭头
  // ----------------------------------------------------------

  // 取一个锚点：内存盒子取「上边中点」（箭头从盒子上方走），
  // 屏幕取「最后一行的左边中点」。
  Player.prototype.anchor = function (name, base) {
    var r;
    if (name === ":screen") {
      var lines = this.screenBody.querySelectorAll(".viz-screen-line");
      var node = lines.length ? lines[lines.length - 1] : this.screenBody;
      r = node.getBoundingClientRect();
      return { x: r.left - base.left + 1, y: r.top - base.top + r.height / 2 };
    }
    var e = this.panels.querySelector('[data-name="' + name + '"]');
    if (!e) return null;
    r = e.getBoundingClientRect();
    return { x: r.left - base.left + r.width / 2, y: r.top - base.top };
  };

  Player.prototype.drawFlows = function (st) {
    if (!this.svg) return;
    this.svg.innerHTML = "";
    var flows = (st && st.flows) || [];
    if (!flows.length) return;

    var base = this.panels.getBoundingClientRect();
    var w = this.panels.clientWidth;
    var h = this.panels.clientHeight;
    this.svg.setAttribute("viewBox", "0 0 " + w + " " + h);
    this.svg.setAttribute("width", w);
    this.svg.setAttribute("height", h);

    for (var k = 0; k < flows.length; k++) {
      var f = flows[k];
      var a = this.anchor(f.from, base);
      var b = this.anchor(f.to, base);
      if (!a || !b) continue;

      // 从盒子出发的箭头从上方绕过去；箭头落在目标的左上方
      var x1 = a.x, y1 = a.y - 5;
      var x2 = b.x - (f.to === ":screen" ? 0 : 0), y2 = b.y - (f.to === ":screen" ? 0 : 5);

      var top = Math.min(y1, y2);
      var mid = (x1 + x2) / 2;
      var lift = top - Math.max(20, Math.min(46, Math.abs(x2 - x1) / 3 + 18));

      var path = document.createElementNS("http://www.w3.org/2000/svg", "path");
      path.setAttribute("d", "M " + x1 + " " + y1 +
                            " Q " + mid + " " + lift + " " + x2 + " " + y2);
      path.setAttribute("class", "viz-arrow");
      this.svg.appendChild(path);

      var dot = document.createElementNS("http://www.w3.org/2000/svg", "circle");
      dot.setAttribute("r", 3.2);
      dot.setAttribute("cx", x2);
      dot.setAttribute("cy", y2);
      dot.setAttribute("class", "viz-arrow-head");
      this.svg.appendChild(dot);
    }
  };

  function root_clear(node) {
    while (node.firstChild) node.removeChild(node.firstChild);
  }

  // ----------------------------------------------------------
  // 挂载
  // ----------------------------------------------------------

  function mountAll() {
    var nodes = document.querySelectorAll("[data-viz]");
    for (var i = 0; i < nodes.length; i++) {
      var node = nodes[i];
      var key = node.getAttribute("data-viz");
      var cfg = (global.VIZ || {})[key];
      if (!cfg) {
        node.innerHTML = '<div class="viz-missing">这个动画没找到数据（<code>' +
                         esc(key) + "</code>）。如果是直接双击打开的网页，" +
                         "检查一下 viz.js 和这个页面在同一个文件夹。</div>";
        continue;
      }
      var p = new Player(node, cfg);
      (global.__VIZ_PLAYERS || (global.__VIZ_PLAYERS = [])).push(p);
    }
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", mountAll);
  } else {
    mountAll();
  }

  global.Viz = { mountAll: mountAll, Player: Player };
})(window);
