module.exports =
  theme:
    options:
      test: true
      verbose: true
      parallel : true
      concurrency : 5
    src: ['./test/scenarios/**/*.js']
    dest: (input) ->
      input.replace(/\.js$/,'.xml')
