function toggleMobileMenu(){
    const mobileMenu = document.querySelector(".hamburger-menu-links");
    const mobileIcon = document.querySelector(".hamburger-icon");
    mobileIcon.classList.toggle("open");
    mobileMenu.classList.toggle("open");
}
console.log(window.outerWidth)