module.exports =
  icons:
    src: 'dist/img/icon/*.png'
    destImg: 'dist/img/<%= pkg.name %>-sprite.png'
    destCSS: 'scss/_sprites.scss'
    cssFormat: 'scss'
    imgPath : '../img/<%= pkg.name %>-sprite.png'
