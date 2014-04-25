(($) ->
  describe 'kalathemeProgress bar is a replacement for Drupal core progress bars' , ->
    testProgressBar = null

    updateCallback = (e)->
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
        expect( $(el).find('#testProgressBar') ).toHaveAttr('aria-describedby','messagetestProgressBar')
        expect($(el).find('.progress-bar') ).toHaveAttr('role', 'progressbar')
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuemin', '0' )
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuemax','100')
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuenow','0')
        expect($(el).find('#messagetestProgressBar')).toExist()
      it 'sets the message and percentage', ->
        spyOn testProgressBar, 'updateCallback'
        box = sandbox()
        box.append testProgressBar.element
        number = 10
        message = 'Test 10% completed'
        testProgressBar.setProgress number, message
        expect(box.find('.progress-bar')).toHaveAttr('aria-valuenow', '10')
        expect(box.find('.progress-bar')).toHaveStyle()

)(jQuery)
