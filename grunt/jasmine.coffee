module.exports =
  specs:
    options:
      specs: 'test/specs/specsBundle.js'
      vendor: [
        'bower_components/jquery/dist/jquery.js'
        'temp/drupaljs/jquery.once.js'
        'temp/drupaljs/drupal.js'
        'temp/drupaljs/ajax.js'
        'temp/drupaljs/autocomplete.js'
        'bower_components/bootstrap/dist/js/bootstrap.js'
        'bower_components/jasmine-ajax/lib/mock-ajax.js'
        'bower_components/jasmine-jquery/lib/jasmine-jquery.js'
      ]
      outfile: 'test/specs/_SpecRunner.html'
      keepRunner: true
