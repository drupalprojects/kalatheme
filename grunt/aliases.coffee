module.exports =

  default: [
    'clean:dist'
    'img'
    'js'
    'css'
    'curl-dir:drupaljs'
    'tests'
  ]
  js: [
    'clean:tempjs'
    'coffeelint:theme'
    'browserify:theme'
    'concat:banner'
    'uglify:js'
  ]
  tests: [
    'coffeelint:tests'
    'browserify:tests'
    'jasmine:specs'
  ]
  img: [
    'imagemin:png'
    'imagemin:jpg'
    'imagemin:gif'
    'sprite:icons'
    'imagemin:icons'
  ]
  css: [
    'sass:dist'
    'autoprefixer:theme'
    # 'csscss:theme'
    # 'csslint:theme'
    'cssmin:theme'
  ]
  develop: [
    'connect:tests'
    'watch'
  ]
  build: [
    'default'
    'clean:build'
    'copy:buildfiles'
  ]
  devrelease: [
    'build'
    'buildcontrol:dev'
    'clean:build'
  ]
  updatefontawesome: [
    'curl-dir:fontawesome'
    'fontawesomejson'
  ]
  ci: [
    'default',
    'phpcs:theme'
  ]
