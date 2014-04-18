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
    files: [{
      expand: true
      cwd: 'dist/js'
      src: ['**/*.js', '**/*.min.js']
      dest: 'dist/js'
      ext: '.min.js'
    }]
