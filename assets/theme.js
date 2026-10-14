/* ============================================================
   换配色 —— 手写，无依赖，断网可用。

   这个文件在 <head> 里加载，不能用 defer：它得赶在浏览器画第一笔之前
   把 data-theme 挂到 <html> 上。晚一步，学生就会看见页面先亮一下再变暗
   （或者反过来），那一闪比没有按钮还烦人。

   颜色写在 style.css 第一节：不挂属性是亮色，data-theme="dark" 是暗色。
   按钮上写什么字也归 style.css 管（跟着这个属性走，见第 3 节末尾），
   所以这里只管三件事：
     一、决定现在该是哪一套——学生按过按钮就听学生的，没按过听系统的
     二、把属性挂上去
     三、按钮按下去的时候换一套，并且记住

   记住用的是 localStorage。file:// 打开的页面在某些浏览器上不让用，
   所以外面套了 try：存不住就算了，这一次照样好使，只是下次开还得再按。
   没有 JS（或者这个文件丢了）的时候页面就是亮色——那是默认值，不是坏掉。
   ============================================================ */

(function () {
  'use strict';

  var KEY = 'cpp-theme';
  var root = document.documentElement;
  var media = window.matchMedia
    ? window.matchMedia('(prefers-color-scheme: dark)')
    : null;

  /* 学生自己按过的那一下。没按过就是 null。 */
  var chosen = null;
  try {
    chosen = window.localStorage.getItem(KEY);
  } catch (e) {
    chosen = null;
  }
  if (chosen !== 'light' && chosen !== 'dark') chosen = null;

  function remember(t) {
    try { window.localStorage.setItem(KEY, t); } catch (e) {}
  }

  /* 第一笔之前就得挂上，所以这一步不放进 DOMContentLoaded */
  root.setAttribute('data-theme',
                    chosen || ((media && media.matches) ? 'dark' : 'light'));

  /* 有 JS 才让按钮显出来。也赶在第一笔之前。 */
  root.classList.add('js');

  /* 学生没自己选过的时候，系统换了就跟着换。他按过按钮，说明他有主意，
     此后系统怎么变都不再动他选的那一套。 */
  if (media && media.addEventListener) {
    media.addEventListener('change', function (e) {
      if (chosen) return;
      root.setAttribute('data-theme', e.matches ? 'dark' : 'light');
    });
  }

  /* 按钮上的字不用这里管，CSS 会跟着 data-theme 自己换 */
  function wire() {
    var btn = document.querySelector('.theme-btn');
    if (!btn) return;
    btn.addEventListener('click', function () {
      var now = root.getAttribute('data-theme') === 'dark' ? 'light' : 'dark';
      root.setAttribute('data-theme', now);
      chosen = now;
      remember(now);
    });
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', wire);
  } else {
    wire();
  }
})();
