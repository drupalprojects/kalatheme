require = patchRequire(global.require)
config = require('./defaultConfig.json')




# config = {}
# utils = require('utils')


#
# fs.readDir('./', (err, files) ->
#   if(err)
#     console.log(err)
#     config = defaultConfig
#   else if _.include(files, 'config.json')
#     config = _.extend(defaultConfig, config)
#     config = require('./config.json')
#   else
#     console.log(files)
# )

casper.test.on "fail", (failure) ->
  casper.capture('./result/casperfailure.png')


casper.test.begin( 'Subtheme generation', 1
, suite = (test) ->

  # Make sure that we can get to the page
  casper.start(config.baseUrl, ->
    test.assertHttpStatus(200)
    casper.capture('./result/home.png')
  )

  # Go to the login page
  casper.thenOpen("#{config.baseUrl}/user", ->
    # finds the login page
    test.assertTitleMatch(/Log in/, 'Finds the login page.')
    # finds the required inputs
    test.assertExists('span.form-required[aria-hidden="true"]'
    , 'finds the asstrk with the right attributes')
    #fill in the form and submit
    @fill('#user-login', {
      name: config.user.name
      pass: config.user.password
    }, true)
  )

  #Start making a subtheme
  casper.waitForSelector('body.logged-in', ->
    @click('#navbar-link-admin-appearance')
  )
  casper.waitForUrl( /admin\/appearance/, ->
    test.assertTextExists('Kalatheme'
    , 'finds that kalatheme is the default theme')
    @click('a[href="/admin/appearance/settings/kalatheme"]')
  )
  settingsForm = 'form[action="/admin/appearance/settings/kalatheme"]'
  casper.waitUntilVisiible(settingsFrom)
  .then(->
    test.assertExists('*[name="build_subtheme"]', 'has a build subtheme checkbox')

    if @getFormValues(settingsForm).build_subtheme
      @fill settingsForm, {
        'build_subtheme': false
      }, false
    @fill settingsForm, {
      'build_subtheme': false
    }, true

    test.assertVisible '#edit-magic', 'shows the autoload assets option'
    test.assertVisible '#edit-subtheme-name', 'shows the theme input'

    @fill settingsForm, {
      'magic': true
      'subtheme_name': 'casper theme'
    }, true
  )
  casper.waitForText('Installing Kalatheme', ->
    test.assertExists('#progress', 'progress bar is shown while waiting.')

  )

  casper.waitForUrl(/settings\/casper_theme/, ->
    test.assertTextExists(
      'Your new subtheme is enabled! Looking good in the neighborhood!!'
      , 'shows completion method.'
    )

  )


  casper.run(->
    test.done()
  )
)
