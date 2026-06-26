/**
 * navbar-toggle.js
 * 顶部导航栏折叠/展开逻辑，对应 #navbar / #navbar-restore 两个元素。
 */

function collapseNavbar() {
  document.getElementById('navbar').classList.add('collapsed');
  document.getElementById('navbar-restore').style.display = 'block';
}

function expandNavbar() {
  document.getElementById('navbar').classList.remove('collapsed');
  document.getElementById('navbar-restore').style.display = 'none';
}
