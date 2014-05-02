module.exports =
  js:
    options:
      banner: '<%= banner %>'
      seperator: ';'
    files:
      'dist/js/<%= pkg.name %>.pkg.js': ['bower_components/bootstrap-accessibility-plugin/plugins/js/bootstrap-accessibility.js','temp/js/**/*.js']
