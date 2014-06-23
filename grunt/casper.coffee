module.exports =
  funcational:
    options:
      test: true
      verbose: true
      parallel : true
      concurrency : 5
    src: ['./test/scenarios/**/*.{js,coffee}']
    dest: 'result/casper-results.xml'
