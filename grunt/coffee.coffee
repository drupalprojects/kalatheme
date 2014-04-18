module.exports =
  theme:
    expand: true
    flatten: true
    src: "src/**/*.coffee"
    dest: "temp/js"
    ext: ".js"

  specs:
    expand: true
    flatten: true
    src: 'test/specs/coffee/**/*.coffee'
    dest: "test/specs"
    ext: ".js"
