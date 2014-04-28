(($) ->
  describe 'kalathemeProgress bar is a replacement for Drupal core progress bars' , ->
    testProgressBar = null

    updateCallback = (percentage, message, obj)->
      console.log 'update called', e
    errorCallback = (e)->
      return e
    testProgressBar = new Drupal.progressBar 'testProgressBar', updateCallback, 'post', errorCallback
    window._testObjs ?= {}
    it 'adds a constructor on Drupal.progressBar', ->
      expect(Drupal.progressBar).toBeDefined()
    describe 'the constructor', ->

      _testObjs.testProgressBar = testProgressBar
      it 'constructs DOM element with matching IDs as a parameter', ->
        expect(testProgressBar).toBeDefined()
        expect(testProgressBar.element).toContainElement('#testProgressBar')
      it 'constructs accessible markup to the page using ARIA Roles', ->
        el = testProgressBar.element
        expect(el).toHaveAttr('aria-live', 'polite')
        expect( $(el).find('#testProgressBar') )
        .toHaveAttr('aria-describedby','messagetestProgressBar')
        expect($(el).find('.progress-bar') ).toHaveAttr('role', 'progressbar')
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuemin', '0' )
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuemax','100')
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuenow','0')
        expect($(el).find('#messagetestProgressBar')).toExist()
    describe 'setProgress method', ->
      box = null
      message = null
      number = null
      beforeEach ->
        box = sandbox()
        box.append testProgressBar.element
        number = 10
        message = 'Test 10% completed'
        spyOn testProgressBar, 'updateCallback'
        testProgressBar.setProgress number, message

      it 'sets the styles, attributes and messages on the DOM', ->
        expect(box.find('.progress-bar'))
        .toHaveAttr('aria-valuenow', String(number))
        expect(box.find('.progress-bar')).toHaveCss({width:"#{number}%"})
        expect(box.find('#messagetestProgressBar')).toHaveText(message)
        expect(box.find('.percentage')).toHaveText("#{number}%")
      it 'calls the callback function', ->
        expect(testProgressBar.updateCallback).toHaveBeenCalled()
        expect(testProgressBar.updateCallback)
        .toHaveBeenCalledWith(10,message,testProgressBar)

    describe 'startMonitoring method',->
      uri = null
      delay = null
      beforeEach ->
        spyOn(testProgressBar, 'sendPing')
      it 'sets a delay and uri to ping and calls Send Ping', ->
        uri = "http://example.com"
        delay = 2000
        expect(testProgressBar.delay).toBeUndefined()
        expect(testProgressBar.uri).toBeUndefined()
        testProgressBar.startMonitoring(uri, delay)
        expect(testProgressBar.uri).toEqual(uri)
        expect(testProgressBar.delay).toEqual(delay)
        expect(testProgressBar.sendPing).toHaveBeenCalled()
    describe 'stopMonitoring method', ->
      it 'clears the uri and timeout', ->
        uri = "http://example.com"
        delay = 2000
        testProgressBar.startMonitoring(uri, delay)
        expect(testProgressBar.uri).toEqual(uri)
        expect(testProgressBar.delay).toEqual(delay)
        testProgressBar.stopMonitoring()
        expect(testProgressBar.uri).not.toEqual(uri)
        expect(testProgressBar.timer).toBeFalsy()

    describe 'sendPing method', ->
      beforeEach ->
        foobar = ->
          return true
        testProgressBar.timer = setTimeout(foobar,500 )
      it 'clears the current current timer if set' , ->
        spyOn($,'ajax')
        testProgressBar.startMonitoring('http://example.com', 200)
        testProgressBar.sendPing()
        expect(testProgressBar.timer).toBeDefined()
        expect($.ajax).toHaveBeenCalled()

    describe 'displayError method', ->
      errorMessage = null
      box = null
      beforeEach ->
        errorMessage = 'A test error has occured'
        box = sandbox()
        box.append testProgressBar.element
        spyOn testProgressBar, 'errorCallback'
        testProgressBar.displayError(errorMessage)
      it 'displays an eror mesage in bootstrap block', ->
        expect(box.find('.alert')).toExist()
        text = box.find('.alert').text()
        expect(text.indexOf(errorMessage)).toBeTruthy()
      it 'should call an error callback', ->
        expect(testProgressBar.errorCallback).toHaveBeenCalled()

)(jQuery)
