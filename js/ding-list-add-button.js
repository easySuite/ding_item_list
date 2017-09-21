/**
 * @file
 * Ding list add button.
 */

(function ($) {
  "use strict";

  Drupal.behaviors.ding_list_add_button = {
    attach: function () {
      // Pull out the content of each ding-list-add-button, and place it in an
      // element, that will be positioned absolute under the add-button when
      // hovered.
      $('.ding-list-add-button').each(function (delta, dlab) {
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
    }
  };
}(jQuery));
