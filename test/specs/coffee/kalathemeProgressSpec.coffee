(($) ->
  describe 'kalathemeProgress bar is a replacement for Drupal core progress bars' , ->
    testProgressBar = null

    updateCallback = (percentage, message, obj)->
      console.log 'update called', e
    errorCallback = (e)->
      conso
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
      url = null
      delay = null
      responses = {}
      beforeEach ->
        jasmine.Ajax.install()
        

    describe 'stopMonitoring method', ->

    describe 'sendPing method', ->

    describe 'displayError method', ->

)(jQuery)
