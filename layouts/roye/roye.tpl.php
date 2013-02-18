<?php
/**
 * @file
 * Template for Kalatheme roye.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display roye clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="roye-container roye-header clearfix panel-panel">
        <div class="roye-container-inner roye-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="roye-container roye-header clearfix panel-panel row-fluid">
        <div class="roye-container-inner roye-header-inner panel-panel-inner">
          <?php print $content['columnhead']; ?>
        </div>
      </div>
    </div>
  </section>

  <section>
    <div class='container'>
      <div class='row-fluid'>
        <div class="roye-container roye-main roye-column-content clearfix">
          <div class="roye-column-content-region roye-content panel-panel span6">
            <div class="roye-column-content-region-inner roye-content-inner panel-panel-inner">
              <?php print $content['column1']; ?>
            </div>
          </div>
          <div class="roye-column-content-region roye-sidebar panel-panel span6">
            <div class="roye-column-content-region-inner roye-sidebar-inner panel-panel-inner">
              <?php print $content['column2']; ?>
            </div>
          </div>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="roye-container roye-column-content roye-column-content-row1 clearfix row-fluid">
        <div class="roye-column-content-region roye-column roye-column1 panel-panel span4">
          <div class="roye-column-content-region-inner roye-column-inner roye-column1-inner panel-panel-inner">
            <?php print $content['footer1']; ?>
          </div>
        </div>
        <div class="roye-column-content-region roye-column roye-column2 panel-panel span4">
          <div class="roye-column-content-region-inner roye-column-inner roye-column2-inner panel-panel-inner">
            <?php print $content['footer2']; ?>
          </div>
        </div>
        <div class="roye-column-content-region roye-column roye-column3 panel-panel span4">
          <div class="roye-column-content-region-inner roye-column-inner roye-column3-inner panel-panel-inner">
            <?php print $content['footer3']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
  
</div><!-- /.roye -->
