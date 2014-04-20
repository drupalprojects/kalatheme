module.exports =
  develop:
    options:
      sourceMap: true
      trace: true
      lineNumbers: true
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
  dist:
    options:
      sourceMap: true
      trace: true
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
