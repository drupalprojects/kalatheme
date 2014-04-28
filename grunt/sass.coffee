module.exports =
  develop:
    options:
      sourcemap: true
      trace: true
      lineNumbers: true
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
  dist:
    options:
      sourcemap: true
      trace: true
    files:
      'dist/css/kalatheme.css' : 'scss/kalatheme.scss'
