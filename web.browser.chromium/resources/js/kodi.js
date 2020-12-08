kodi.gui.dialogs.OK.ShowAndGetInput.NonBlocked = function(heading, text, cb) {
  window.kodiQuery({
    request: '{"func":"kodi.gui.dialogs.OK.ShowAndGetInput", "params": { "heading": "'+heading+'", "text": "'+text+'" } }',
    onSuccess: function(response) {
      if (cb)
        cb();
    },
    onFailure: function(error_code, error_message) {
      alert(error_message);
    }
  });
};

kodi.gui.dialogs.TextViewer.Show.NonBlocked = function(heading, text, cb) {
  window.kodiQuery({
    request: '{"func":"kodi.gui.dialogs.TextViewer.Show", "params": { "heading": "'+heading+'", "text": "'+text+'" } }',
    onSuccess: function(response) {
      if (cb)
        cb();
    },
    onFailure: function(error_code, error_message) {
      alert(error_message);
    }
  });
};

kodi.gui.dialogs.YesNo.ShowAndGetInput.NonBlocked = function(heading, text, cb) {
  window.kodiQuery({
    request: '{"func":"kodi.gui.dialogs.YesNo.ShowAndGetInput", "params": { "heading": "'+heading+'", "text": "'+text+'", "no": "", "yes": "" } }',
    onSuccess: function(response) {
      if (cb)
      {
        var obj = JSON.parse(response);
        cb(obj.returns.ret, obj.returns.canceled);
      }
    },
    onFailure: function(error_code, error_message) {
      alert(error_message);
    }
  });
};

kodi.gui.dialogs.YesNo.ShowAndGetInput.NonBlockedDiffNoYes = function(heading, text, noLabel, yesLabel, cb) {
  window.kodiQuery({
    request: '{"func":"kodi.gui.dialogs.YesNo.ShowAndGetInput", "params": { "heading": "'+heading+'", "text": "'+text+'", "no": "'+noLabel+'", "yes": "'+yesLabel+'" } }',
    onSuccess: function(response) {
      if (cb)
      {
        var obj = JSON.parse(response);
        cb(obj.returns.ret, obj.returns.canceled);
      }
    },
    onFailure: function(error_code, error_message) {
      alert(error_message);
    }
  });
};
