from environment import *
from libraries.ADLib.buildutil.ScreenMaker import *

working_directory = os.path.dirname(os.path.realpath(__file__))
screen_directory = os.path.join(working_directory, 'Resources')

env.win_executable_dir = os.path.join(working_directory, 'proj.qt/bin/debug')

devices = [ #'IPhone3GS',
		'IPhone4GS',
		'IPhone5',
		'IPhone6',
		'IPhone6Plus',
		'IPad2',
	'AmazonScreenShots',
    #'IPad4',
    #'Galaxy_Ace2',
    #'Galaxy_Mini2',
    #'HTC_One_S',
    #'Motorola_Droid',
    #'Galaxy_Y',
    #'Galaxy_Tab2_7',
    #'Galaxy_Tab2_10',
	#	'WindowsPhone_WXGA'#,
	#'Kindle_Fire',
    #'Google_Nexus_10',
    #'Sony_Xperia_Z',
    #'Kindle_Fire',
    #'Microsoft_Surface_10',
    #'HD_720p',
    #'Custom'
]

languages = ['en','ru']#, 'uk']
orientations = ['landscape']

maker = ScreenMaker(env, 'MathIsFun2.exe', screen_directory)
maker.makeScreens(devices, orientations, languages)

