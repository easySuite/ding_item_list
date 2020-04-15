(function($) {
  'use strict';

  Drupal.behaviors.ding_item_list = {
    attach: function (context) {
      $('.ding-item-list', context).each(function () {
        var hash = $(this).data('hash');
        var ding_item_list = (Drupal.settings[hash] !== undefined) ? Drupal.settings[hash] : '';
        if (ding_item_list !== '') {
          $.getJSON(Drupal.settings.basePath + Drupal.settings.pathPrefix + 'ding_item_list/' + hash, {content: ding_item_list}, function (content) {
            $('.pane-ding-item-list', context).find('[data-hash=' + hash + ']').replaceWith(content);
            // This is a hack. Need to implement drupal_ajax_command.
            // so, the behaviours will be triggered on DOM alter.
            Drupal.attachBehaviors('.ding-item-list-item');

            let $list = $('.ding-list-add-button');

            $list.each(function (delta, dlab) {
              $(this).removeClass('hidden');

              let $buttons = $('.buttons', dlab)
                  .css({
                    position: 'absolute',
                    display: 'none'
                  })
                  .addClass('dlab-breakout-button')
                  .appendTo('body');

              $(dlab).find('.content').empty();

              // The .trigger is the "hit area" of the hover effect.
              $('.trigger', dlab).bind('mouseenter', function () {
                $buttons.css($(this).offset())
                    .css('display', 'block');
                $(dlab).addClass('showing');
              });

              $buttons.bind('mouseleave', function () {
                $buttons.css('display', 'none');
                $(dlab).removeClass('showing');
              });
            });


          });
        }
      });
    }
  };
} (jQuery));
