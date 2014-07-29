from libraries.ADLib.modules_android import *

from libraries.ADLib.buildutil.AndroidProject import *
from libraries.ADLib.buildutil.Modules import  *

modules = Modules()
loadModules(modules)

working_directory = os.path.dirname(os.path.realpath(__file__))

from environment import *

project = AndroidProject(env)
project.modules = modules

#Modules
project.addModule('android_ADAds_AdMob')
project.addModule('android_ADStatistics_Flurry')
project.addModule('android_ADLanguage')
project.addModule('android_ADNotification')
project.addModule('android_ADBrowser')
project.addModule('android_ADFacebook_None')
project.addModule('android_ADHttp_None')

project.project_name = 'Fun Math 2*2'
project.package_name = 'x4enjoy.funmath.multiplication.table'
project.version_name = "1.2"
project.version_code = "1"
project.resources_compile_task = createResourceCompilerTask(
    os.path.join(working_directory, 'Resources_storage'),
    'mif_mul',
    'android',
    ['en']
)

project.signature = createAndroidSignature('build.android.mul/math-is-fun.jks',
											'math-is-fun',
                                           '9aTFpP7DMBKRmiHc3RDT', '9aTFpP7DMBKRmiHc3RDT')
project.res_dir = 'build.android.mul/res'
project.working_directory = working_directory
project.addLookUpDir('libraries/ADLib/platform/Android/cpp')
project.addLookUpDir('libraries/ADLib/platform/Android/libs')
project.addLookUpDir('libraries/ADLib/platform/Android/java')
project.addLookUpDir('libraries/ADLib/platform/Android/res')
project.addLookUpDir('libraries/ADLib/src')
project.addLookUpDir('Classes')
project.main_activity_definition = project.getMainActivityDeclaration()

project.addCppDir('libraries/ADLib/src')
project.addCppDir('Classes')
