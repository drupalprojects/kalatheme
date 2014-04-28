module.exports =
  js:
    options:
      banner: '<%= banner %>'
      seperator: ';'
    files:
      'dist/js/<%= pkg.name %>.pkg.js': ['temp/js/**/*.js']
