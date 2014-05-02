module.exports =
  options:
    sourcemap: true
    trace: true
    loadPath: [
      'bower_components'
    ]
  develop:
    options:
      lineNumbers: true
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
  dist:
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
