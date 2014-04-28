module.exports =
  js:
    options:
      compress: true
      mangle: true
      sourceMap: true
      dropconsole: true
      sourcemap: true
      sourceMapName: 'dist/js/<%=  pkg.name %>.map'
      banner: '<%= banner %>'
    files:
      'dist/js/<%= pkg.name %>.pkg.min.js' : 'dist/js/<%= pkg.name %>.pkg.js'
