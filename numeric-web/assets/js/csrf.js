// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License


import Cookies from 'js-cookie'


function csrfSafeMethod(method) {
    // these HTTP methods do not require CSRF protection
    return (/^(GET|HEAD|OPTIONS|TRACE)$/.test(method));
}

function csrf($) {
  var csrftoken = Cookies.get('csrfmiddlewaretoken');
  if (!csrftoken) {
    csrftoken = $('[name=csrfmiddlewaretoken]').val();
  }

  $.ajaxSetup({
      beforeSend: function(xhr, settings) {
          if (!csrfSafeMethod(settings.type) && !this.crossDomain) {
              xhr.setRequestHeader("X-CSRFToken", csrftoken);
          }
      }
  });
}

export default csrf;
