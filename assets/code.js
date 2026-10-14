/* ============================================================
   代码块装饰 —— 手写，无依赖，断网可用。

   页面上写的是光秃秃的一坨：

       <pre><code>#include <iostream>

       int main() {
           return 0;
       }</code></pre>

   打开页面之后，这段 JS 把它变成：

       外壳（.code）+ 顶上一条（.code-bar：语言 + 复制按钮）
       + 逐行切开（.line，行号是 .ln 那个链接）

   源文件里的字一个都没动——行号是加载后在浏览器里套上去的，
   页面里 <pre><code> 的原文和源文件逐字一致。

   三样东西一律不碰：
       pre.out   预期输出 —— 机器打的话，不是代码
       pre.diag  编译器输出 —— 同上
       裸 <pre>  终端转录 —— 连 <code> 都没有，选都选不中
   它们不是代码，是机器说的话，套上行号和复制按钮就串味了。

   代价说明白：关掉 JS 就没有行号，也没有复制按钮。
   复制按钮本来就得靠 JS；行号本来不必，这里也让它靠了 JS。

   ── 依赖的类名 ──
   .code / .code-bar / .code-lang / .copy / .line / .ln / .tok-*
   样式在 style.css 第 15 节。行号锚点是 c<块号>-<行号>，课里的
   id 是 l1-1 / s1-1-1 / e1-1 这几种，c 开头一个都没有，撞不上。
   ============================================================ */

(function () {
  'use strict';

  /* ── 着色：认字的短表 ────────────────────────────────
     这张表只决定「哪个字刷什么颜色」。认错了一个字，顶多它不上色，
     文本内容一个字都不会变，所以动不了行号，也动不了复制出去的东西。
     整段着色不想要，删掉 colorize 那一段即可，其余照常。 */
  var KEYWORDS = [
    'int', 'double', 'char', 'bool', 'void', 'long', 'short', 'float',
    'unsigned', 'signed', 'const', 'constexpr', 'static', 'inline', 'auto',
    'if', 'else', 'for', 'while', 'do', 'return', 'break', 'continue',
    'switch', 'case', 'default', 'struct', 'class', 'public', 'private',
    'new', 'delete', 'nullptr', 'true', 'false', 'sizeof',
    'using', 'namespace', 'include', 'define', 'ifndef', 'ifdef', 'endif', 'pragma'
  ];

  /* 名字后面跟着 ( 的，当成函数名。main、swap 这些都不是关键字，
     不认这一条它们就跟普通变量一个色。 */
  var RE_WORD = /^[A-Za-z_][A-Za-z0-9_]*/;
  var RE_NUM = /^[0-9][0-9A-Za-z_.]*/;
  var RE_DIRECTIVE = /^#[ \t]*[A-Za-z_]+/;
  var RE_ANGLE = /^<[A-Za-z_][A-Za-z0-9_./]*>/;
  var RE_CALL = /^[ \t]*\(/;

  /* 往回找最近一个不是空白的 token。#include 和它后面那个 <iostream>
     中间隔着一个空格，那个空格自己也是一个 token，所以不能只看最后一个。 */
  function lastMeaningful(out) {
    for (var k = out.length - 1; k >= 0; k--) {
      if (/[^ \t\r\n]/.test(out[k].text)) return out[k];
    }
    return null;
  }

  /* 把一段源码切成 [{text, cls}]。cls 是 null 就是原色。
     换行原样留在 text 里，交给分行那一步去切——这里不管行。 */
  function colorize(src) {
    var out = [], i = 0;

    function emit(text, cls) { out.push({ text: text, cls: cls }); }

    while (i < src.length) {
      var rest = src.slice(i);
      var c = src.charAt(i);
      var m, e;

      /* // 行注释：到行尾为止。结尾那个换行不算进来，它得留给分行 */
      if (c === '/' && src.charAt(i + 1) === '/') {
        e = src.indexOf('\n', i);
        if (e < 0) e = src.length;
        emit(src.slice(i, e), 'tok-cm');
        i = e;
        continue;
      }

      /* 块注释：跨行，所以不能交给分行那一步自己切 */
      if (c === '/' && src.charAt(i + 1) === '*') {
        e = src.indexOf('*/', i + 2);
        e = (e < 0) ? src.length : e + 2;
        emit(src.slice(i, e), 'tok-cm');
        i = e;
        continue;
      }

      /* 字符串和字符字面量。得认 \" 这种转义，不然 "a\"" 会提前收尾，
         后面半句全被当成字符串刷了。 */
      if (c === '"' || c === "'") {
        var j = i + 1;
        while (j < src.length && src.charAt(j) !== c) {
          if (src.charAt(j) === '\\') j++;
          j++;
        }
        e = Math.min(j + 1, src.length);
        emit(src.slice(i, e), 'tok-str');
        i = e;
        continue;
      }

      /* 数字。10、3.14、0x1F 都收成一段 */
      if (c >= '0' && c <= '9') {
        m = RE_NUM.exec(rest);
        emit(m[0], 'tok-num');
        i += m[0].length;
        continue;
      }

      /* #include 后面那个 <iostream> 不是标签，是个文件名，按字符串刷。
         不认这一条，那对尖括号就成了整行里唯一没颜色的东西。 */
      if (c === '<') {
        m = RE_ANGLE.exec(rest);
        var prev = lastMeaningful(out);
        if (m && prev && /^#[ \t]*include$/.test(prev.text)) {
          emit(m[0], 'tok-str');
          i += m[0].length;
          continue;
        }
      }

      /* 预处理指令整条算关键字：#include、#define、#pragma */
      if (c === '#') {
        m = RE_DIRECTIVE.exec(rest);
        if (m) {
          emit(m[0], 'tok-kw');
          i += m[0].length;
          continue;
        }
      }

      /* 名字：词组、关键字、函数名，都不是就原色 */
      if (/[A-Za-z_]/.test(c)) {
        m = RE_WORD.exec(rest);
        var word = m[0];
        var after = src.slice(i + word.length);

        /* 「using namespace」是两个词连起来才算一整条 */
        if (word === 'using') {
          var ns = /^[ \t]+namespace\b/.exec(after);
          if (ns) {
            emit('using' + ns[0], 'tok-kw');
            i += word.length + ns[0].length;
            continue;
          }
        }
        if (KEYWORDS.indexOf(word) >= 0) {
          emit(word, 'tok-kw');
          i += word.length;
          continue;
        }
        if (RE_CALL.test(after)) {
          emit(word, 'tok-fn');
          i += word.length;
          continue;
        }
        emit(word, null);
        i += word.length;
        continue;
      }

      /* 其它一律原色，一个字符一个字符地攒 */
      var last = out[out.length - 1];
      if (last && last.cls === null) last.text += c;
      else emit(c, null);
      i++;
    }
    return out;
  }

  /* ── 分行 ────────────────────────────────────────────
     把 token 按换行切成一行一簇。切出来的片段里不含换行——行与行之间
     那个真换行由 build 单独补上，这样 textContent 和原文一字不差。 */
  function toLines(tokens) {
    var lines = [[]];
    tokens.forEach(function (tk) {
      var parts = tk.text.split('\n');
      for (var k = 0; k < parts.length; k++) {
        if (k) lines.push([]);
        if (parts[k]) lines[lines.length - 1].push({ text: parts[k], cls: tk.cls });
      }
    });
    return lines;
  }

  /* ── 摆出来 ──────────────────────────────────────────
     每一行是一个 <span class="line">，行号是它开头的 <a class="ln">。
     行号用 display:block 摆出来（样式表里），不是文本里的换行，
     所以把它摘掉之后剩下的就是纯代码。 */
  function build(code, lines, no) {
    var frag = document.createDocumentFragment();

    lines.forEach(function (parts, li) {
      var n = li + 1;
      var id = 'c' + no + '-' + n;

      var line = document.createElement('span');
      line.className = 'line';
      line.id = id;

      /* 行号是个链接：报错说「第 11 行」，点它就跳过去。
         屏幕阅读器上把它藏掉——一屏念几十个数字，比不念还糟；
         键盘也跳过它（正文才是要读的东西）。 */
      var ln = document.createElement('a');
      ln.className = 'ln';
      ln.href = '#' + id;
      ln.tabIndex = -1;
      ln.setAttribute('aria-hidden', 'true');
      ln.textContent = String(n);
      line.appendChild(ln);

      parts.forEach(function (p) {
        if (!p.cls) {
          line.appendChild(document.createTextNode(p.text));
          return;
        }
        var s = document.createElement('span');
        s.className = p.cls;
        s.textContent = p.text;
        line.appendChild(s);
      });

      frag.appendChild(line);
      /* 行与行之间留一个真换行。它是给 textContent 留的，
         视觉上的换行是 .line 的 display:block 摆出来的 */
      if (li < lines.length - 1) frag.appendChild(document.createTextNode('\n'));
    });

    code.textContent = '';
    code.appendChild(frag);
  }

  /* ── 顶上那一条 ──────────────────────────────────────
     语言标签 + 复制按钮。 */
  function langOf(pre) {
    /* 块自己写了 data-lang 就听它的：<pre data-lang="终端"> */
    var own = pre.getAttribute('data-lang');
    if (own) return own;
    /* 看不出来就是 C++。00 里有三块是敲给终端的命令（g++ …），照这个样子认。
       认错了顶多标签上写错两个字，底下的代码一个字不动。 */
    var head = (pre.textContent.split('\n')[0] || '');
    if (/^[ \t]*(\$|g\+\+|gcc|clang|make|\.\/|cat |ls |cd )/.test(head)) return '终端';
    return 'C++';
  }

  function bar(lang, code) {
    var el = document.createElement('div');
    el.className = 'code-bar';

    var tag = document.createElement('span');
    tag.className = 'code-lang';
    tag.textContent = lang;
    el.appendChild(tag);

    var btn = document.createElement('button');
    btn.className = 'copy';
    btn.type = 'button';
    btn.textContent = '复制';
    btn.setAttribute('aria-label', '复制这段代码');
    btn.addEventListener('click', function () { doCopy(btn, code); });
    el.appendChild(btn);

    return el;
  }

  /* ── 复制 ────────────────────────────────────────────
     复制出去的是纯代码，不带行号：所以先把克隆体里的 .ln 摘掉再取文本。
     直接读 textContent 会把行号一起带走。 */
  function codeText(code) {
    var clone = code.cloneNode(true);
    var nums = clone.querySelectorAll('.ln');
    for (var i = 0; i < nums.length; i++) nums[i].parentNode.removeChild(nums[i]);
    return clone.textContent.replace(/\s+$/, '');
  }

  /* 课是双击 HTML 打开的（file://），那里 navigator.clipboard 常常不在，
     所以留一条老路：把文本塞进屏幕外的 textarea 再选中它。这条不能省。 */
  function legacyCopy(text) {
    var ta = document.createElement('textarea');
    ta.value = text;
    ta.setAttribute('readonly', '');
    ta.style.position = 'fixed';
    ta.style.top = '-1000px';
    ta.style.opacity = '0';
    document.body.appendChild(ta);
    ta.select();
    ta.setSelectionRange(0, text.length);
    var ok = false;
    try { ok = document.execCommand('copy'); } catch (e) { ok = false; }
    document.body.removeChild(ta);
    return ok;
  }

  function flash(btn, text, ok) {
    if (!btn._label) btn._label = btn.textContent;
    btn.textContent = text;
    if (ok) btn.setAttribute('data-copied', '1');
    else btn.removeAttribute('data-copied');
    clearTimeout(btn._t);
    btn._t = setTimeout(function () {
      btn.textContent = btn._label;
      btn.removeAttribute('data-copied');
    }, ok ? 1600 : 2600);
  }

  function doCopy(btn, code) {
    var text = codeText(code);
    if (navigator.clipboard && navigator.clipboard.writeText) {
      navigator.clipboard.writeText(text).then(function () {
        flash(btn, '已复制', true);
      }, function () {
        var ok = legacyCopy(text);
        flash(btn, ok ? '已复制' : '复制失败', ok);
      });
      return;
    }
    var ok = legacyCopy(text);
    flash(btn, ok ? '已复制' : '复制失败', ok);
  }

  /* ── 一块一块地装 ──────────────────────────────────── */
  var blockNo = 0;

  function decorate(code) {
    var pre = code.parentNode;
    if (!pre || pre.tagName !== 'PRE') return;
    /* 已经装过的跳过（比如以后加了个东西让这段跑两遍） */
    if (pre.parentNode && pre.parentNode.className === 'code') return;

    var lang = langOf(pre);
    var src = code.textContent;
    /* 空块不装。装上去只会剩一个孤零零的行号「1」。 */
    if (!src.trim()) return;
    var lines = toLines(colorize(src));

    /* 先量语言再动 DOM——build 会把行号写进 textContent，
       量晚了一步，第一行会变成「1#include <iostream>」。 */
    build(code, lines, ++blockNo);

    var shell = document.createElement('div');
    shell.className = 'code';
    pre.parentNode.insertBefore(shell, pre);
    shell.appendChild(bar(lang, code));
    shell.appendChild(pre);
  }

  function start() {
    var codes = document.querySelectorAll('pre:not(.out):not(.diag) > code');
    for (var i = 0; i < codes.length; i++) decorate(codes[i]);
  }

  /* 脚本摆在 </body> 前，那时 DOM 已经好了，直接跑；
     挪到 <head> 去也不会失灵。 */
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', start);
  } else {
    start();
  }
})();
