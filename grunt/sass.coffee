module.exports =
  theme:
    options:
      imagePath: '<%= imagemin.theme.dest %>'
      sourceComments: 'map'
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
