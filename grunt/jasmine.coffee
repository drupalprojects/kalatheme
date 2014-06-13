module.exports =
  specs:
    options:
      specs: 'test/tests.js'
      vendor: [
        'bower_components/jquery/dist/jquery.js'
        'temp/drupaljs/jquery.once.js'
        'temp/drupaljs/drupal.js'
        'temp/drupaljs/ajax.js'
        'temp/drupaljs/autocomplete.js'
      ]
      outfile: 'test/specs/_SpecRunner.html'
      keepRunner: true
