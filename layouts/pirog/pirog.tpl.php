<?php
/**
 * @file
 * Template for Kalatheme pirog.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display pirog clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  
  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="pirog-container pirog-header clearfix panel-panel">
        <div class="pirog-container-inner pirog-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="pirog-container pirog-header clearfix panel-panel row-fluid">
        <div class="pirog-container-inner pirog-header-inner panel-panel-inner">
          <?php print $content['feature']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="pirog-container pirog-header clearfix panel-panel row-fluid">
        <div class="pirog-container-inner pirog-header-inner panel-panel-inner">
          <?php print $content['main']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="pirog-container pirog-header clearfix panel-panel row-fluid">
        <div class="pirog-container-inner pirog-header-inner panel-panel-inner">
          <?php print $content['second']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="pirog-container pirog-column-content pirog-column-content-row1 clearfix row-fluid">
        <div class="pirog-column-content-region pirog-column pirog-column1 panel-panel span4">
          <div class="pirog-column-content-region-inner pirog-column-inner pirog-column1-inner panel-panel-inner">
            <?php print $content['footer1']; ?>
          </div>
        </div>
        <div class="pirog-column-content-region pirog-column pirog-column2 panel-panel span4">
          <div class="pirog-column-content-region-inner pirog-column-inner pirog-column2-inner panel-panel-inner">
            <?php print $content['footer2']; ?>
          </div>
        </div>
        <div class="pirog-column-content-region pirog-column pirog-column3 panel-panel span4">
          <div class="pirog-column-content-region-inner pirog-column-inner pirog-column3-inner panel-panel-inner">
            <?php print $content['footer3']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
</div><!-- /.pirog -->
