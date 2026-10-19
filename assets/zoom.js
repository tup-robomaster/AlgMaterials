/* 正文图片的放大查看。

   双击带 .zoomable 的图片，在页面里铺一层不透明背景，图片按窗口大小放下；
   滚轮继续放大或缩小（0.25 倍到 8 倍），放大后按住图片拖动可以平移；
   再次双击、点背景或按 Esc 还原。页头的 logo 没有 .zoomable，不参与。

   不联网、不依赖别的脚本。图片仍用原来的 src，不生成副本。 */
(function () {
  'use strict';

  var layer = null;
  var pic = null;
  var box = null;
  var scale = 1;          // 1 表示刚好放进窗口
  var baseW = 0;
  var baseH = 0;
  var lastFocus = null;
  var drag = null;        // 拖动中记录起点和当时的滚动位置

  function clamp(v, lo, hi) {
    return v < lo ? lo : (v > hi ? hi : v);
  }

  function draw() {
    pic.style.width = Math.round(baseW * scale) + 'px';
    pic.style.height = Math.round(baseH * scale) + 'px';
  }

  function fit() {
    // 图片按原始比例缩到窗口的 94% × 90%，不放大到超过原始尺寸
    var vw = window.innerWidth * 0.94;
    var vh = window.innerHeight * 0.90;
    var k = Math.min(vw / pic.naturalWidth, vh / pic.naturalHeight, 1);
    baseW = Math.max(1, Math.round(pic.naturalWidth * k));
    baseH = Math.max(1, Math.round(pic.naturalHeight * k));
    scale = 1;
    draw();
  }

  function close() {
    if (!layer) return;
    drag = null;
    document.removeEventListener('keydown', onKey);
    layer.remove();
    layer = null;
    pic = null;
    box = null;
    document.body.style.overflow = '';
    if (lastFocus && lastFocus.focus) lastFocus.focus();
    lastFocus = null;
  }

  function onKey(e) {
    if (e.key === 'Escape') close();
  }

  function onPointerDown(e) {
    if (e.button !== 0) return;
    drag = {
      x: e.clientX,
      y: e.clientY,
      left: layer.scrollLeft,
      top: layer.scrollTop
    };
    try { pic.setPointerCapture(e.pointerId); } catch (err) {}
  }

  function onPointerMove(e) {
    if (!drag) return;
    var dx = e.clientX - drag.x;
    var dy = e.clientY - drag.y;
    if (Math.abs(dx) < 3 && Math.abs(dy) < 3) return;
    layer.classList.add('dragging');
    layer.scrollLeft = drag.left - dx;
    layer.scrollTop = drag.top - dy;
    e.preventDefault();
  }

  function onPointerUp(e) {
    if (!drag) return;
    drag = null;
    layer.classList.remove('dragging');
    try {
      if (e.pointerId !== undefined && pic.hasPointerCapture(e.pointerId)) {
        pic.releasePointerCapture(e.pointerId);
      }
    } catch (err) {}
  }

  function open(src, alt, from) {
    if (layer) close();
    lastFocus = from;

    layer = document.createElement('div');
    layer.className = 'zoom-layer';

    box = document.createElement('div');
    box.className = 'zoom-inner';

    pic = document.createElement('img');
    pic.alt = alt || '';
    pic.draggable = false;
    pic.src = src;
    box.appendChild(pic);
    layer.appendChild(box);
    document.body.appendChild(layer);
    document.body.style.overflow = 'hidden';

    if (pic.complete && pic.naturalWidth) {
      fit();
    } else {
      pic.addEventListener('load', fit, { once: true });
    }

    // 双击放大后的图片：还原。点背景（不是图片本身）：同样还原。
    layer.addEventListener('dblclick', close);
    layer.addEventListener('click', function (e) {
      if (e.target !== pic) close();
    });
    layer.addEventListener('wheel', function (e) {
      e.preventDefault();
      scale = clamp(scale * Math.exp(-e.deltaY * 0.0015), 0.25, 8);
      draw();
    }, { passive: false });

    pic.addEventListener('dragstart', function (e) { e.preventDefault(); });
    pic.addEventListener('pointerdown', onPointerDown);
    pic.addEventListener('pointermove', onPointerMove);
    pic.addEventListener('pointerup', onPointerUp);
    pic.addEventListener('pointercancel', onPointerUp);

    document.addEventListener('keydown', onKey);
  }

  document.addEventListener('dblclick', function (e) {
    var t = e.target;
    if (!(t instanceof HTMLImageElement)) return;
    if (!t.classList.contains('zoomable')) return;
    e.preventDefault();
    open(t.currentSrc || t.src, t.alt, t);
  });

  document.addEventListener('keydown', function (e) {
    var t = e.target;
    if (!(t instanceof HTMLImageElement)) return;
    if (!t.classList.contains('zoomable')) return;
    if (e.key !== 'Enter' && e.key !== ' ') return;
    e.preventDefault();
    open(t.currentSrc || t.src, t.alt, t);
  });
})();
