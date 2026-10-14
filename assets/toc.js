/* ============================================================
   目录高亮 —— 手写，无依赖，断网可用。

   左边那条目录里，你现在读到哪一小节，哪一行就变蓝。
   没有它，目录就是一长串文字；有了它，你随时知道自己在哪，
   也知道下面还有什么。

   规则就两条。

   一、滚动的时候，找出「已经越过判定线的那一节里最靠下的一个」，就是它。
   判定线不是屏幕顶边——顶栏压在上面呢。点目录跳过去之后，目标标题停在
   CSS 给它的 scroll-margin-top 那个位置（顶栏下沿再往下一点），得让它算
   「已经到这儿了」，不然点完它会停在上一节。所以这条线 =
   顶栏高 + scroll-margin-top + 8px 余量，两个数都从页面上量，不写死。

   二、滚到页尾了，就把最后一条点亮。最后一节常常短得顶不到那条
   90px 的线（目录页的「附录」就是这样），只靠第一条的话，你点它
   跳到页尾，亮的却是它前面那一节。

   课名（.toc-l）和小节（.toc-sub）不并列：小节亮的时候，它属于的
   那一课也一起亮——你在 1.2 的某小节里这件事，比「你在哪个小节」更值得看。
   ============================================================ */

(function () {
  'use strict';

  var toc = document.querySelector('.toc');
  if (!toc) return;

  var lessons = [].slice.call(toc.querySelectorAll('a.toc-l'));
  if (!lessons.length) return;

  /* 把目录按课分组：每一课 = 课名 + 跟在它后面的若干小节 */
  var groups = lessons.map(function (head) {
    var subs = [];
    for (var n = head.nextElementSibling; n; n = n.nextElementSibling) {
      if (n.classList && n.classList.contains('toc-l')) break;
      if (n.tagName === 'A' && n.classList.contains('toc-sub')) subs.push(n);
    }
    return { head: head, links: [head].concat(subs) };
  });

  /* 摊平成一个数组，顺便记住每一项属于哪一课 */
  var items = [];
  groups.forEach(function (g, gi) {
    g.links.forEach(function (a) {
      var id = (a.getAttribute('href') || '').slice(1);
      items.push({ el: document.getElementById(id), a: a, gi: gi });
    });
  });
  /* 目录里有 href 写错、页面上找不到对应 id 的，直接扔掉——
     宁可不亮，也不要在滚动时抛异常把后面全带停 */
  items = items.filter(function (it) { return it.el; });
  if (!items.length) return;

  var shown = -2;

  /* 判定线。量出来而不是写死——顶栏多高、落点留多少，style.css 说了算，
     这里跟着走。窗口大小变了重算一次，字号跟着变的时候也不会失准。 */
  var line = 90;
  function measure() {
    var bar = document.querySelector('.topbar');
    var probe = document.querySelector('main [id]');
    line = (bar ? bar.offsetHeight : 0) +
           (probe ? (parseFloat(getComputedStyle(probe).scrollMarginTop) || 0) : 0) + 8;
  }

  function atBottom() {
    var doc = document.documentElement;
    /* 页面短到不用滚的时候，它永远「在底部」。那样最后一条会一直亮着，
       所以先问一句：这页到底滚不滚得动。 */
    if (doc.scrollHeight <= window.innerHeight) return false;
    return window.innerHeight + window.scrollY >= doc.scrollHeight - 2;
  }

  function update() {
    var cur = -1;
    for (var i = 0; i < items.length; i++) {
      if (items[i].el.getBoundingClientRect().top <= line) cur = i;
    }
    /* 滚到底了，就把最后一条点亮。最后一节常常短得顶不到上面那条 90px 的线，
       光靠上面这个判断，你点它跳到页尾，亮的却是它前面那一节。 */
    if (atBottom()) cur = items.length - 1;

    if (cur === shown) return;
    shown = cur;

    var curGroup = cur >= 0 ? items[cur].gi : -1;

    items.forEach(function (it, i) {
      it.a.classList.toggle('here', i === cur);
    });
    groups.forEach(function (g, gi) {
      g.head.classList.toggle('here', gi === curGroup);
    });
  }

  var queued = false;
  function onScroll() {
    if (queued) return;
    queued = true;
    window.requestAnimationFrame(function () {
      queued = false;
      update();
    });
  }

  window.addEventListener('scroll', onScroll, { passive: true });
  window.addEventListener('resize', function () {
    measure();
    onScroll();
  });
  measure();
  update();
})();
