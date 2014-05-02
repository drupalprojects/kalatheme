module.exports =
  theme:
    src: 'dist/css/kalatheme.css'
    options:
      csslintrc: ".csslintrc"
      force: true
      formatters: [
        { id: 'text', dest:'temp/report/csslint.txt'}
      ]
