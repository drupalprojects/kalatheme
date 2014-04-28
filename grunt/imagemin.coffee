module.exports =
  theme:
    files: [
      {
        expand: true,
        cwd: 'img-src'
        src: ['**/*.{png,jpg,gif}']
        dest: 'dist/img'
      }
    ]
  icons:
    files: {
      'dist/img/kalatheme-sprite.png' : 'dist/img/kalatheme-sprite.png'
    }
