#
# The macOS half of the package. An app installs it through
# `flutter_install_all_macos_pods`, which finds it from the pubspec dependency
# alone — there is nothing to add to the app's Runner.xcodeproj.
#
require 'yaml'

# One source of truth for the version. A podspec that keeps its own copy drifts
# from the pubspec the moment anyone forgets, and the drift shows up in every
# consuming app's Podfile.lock.
pubspec = YAML.load_file(File.join(__dir__, '..', 'pubspec.yaml'))

Pod::Spec.new do |s|
  s.name             = 'myo_window_chrome'
  s.version          = pubspec['version']
  s.summary          = 'The title bar a dark Myoland macOS window wears.'
  s.description      = <<-DESC
One copy of the window chrome, shared by every app that uses it, so no two
windows on the same desktop can drift apart.
                       DESC
  s.homepage         = 'https://github.com/Myoland/packages-flutter'
  s.license          = { :type => 'Private' }
  s.authors          = 'Myoland'
  s.source           = { :path => '.' }
  # Sources only: the Swift Package beside it carries a Tests directory that
  # must not compile into the app.
  s.source_files     = 'myo_window_chrome/Sources/myo_window_chrome/**/*.swift'
  s.osx.dependency 'FlutterMacOS'
  s.osx.deployment_target = '13.0'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '6.0'
end
