import xbmcaddon
import xbmcgui

if __name__ == '__main__':
    dialog = xbmcgui.WindowXMLDialog('browser.xml', xbmcaddon.Addon().getAddonInfo('path'), 'default', '1080p')
    dialog.doModal()
    del dialog
