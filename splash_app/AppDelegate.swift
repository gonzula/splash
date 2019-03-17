//
//  AppDelegate.swift
//  splash
//
//  Created by Gonzo Fialho on 02/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication,
                     didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {

        installExamples()
        window = UIWindow(frame: UIScreen.main.bounds)
        window!.rootViewController = ViewController()
        window!.makeKeyAndVisible()

        return true
    }

    func installExamples() {
        let fileNames = ["Age", "Leap Year", "Quadratic Solver"].map {$0 + ".splash"}
        let documentsPath = FileManager.documentsDirectory.path
        let examplesPath = (documentsPath as NSString).appendingPathComponent("Examples")
        let examplesURL = URL(fileURLWithPath: examplesPath)
        var resourceValues = URLResourceValues()
        resourceValues.isExcludedFromBackup = true
        try? (examplesURL as NSURL).setResourceValue(true, forKey: .isExcludedFromBackupKey)
        try? FileManager.default.createDirectory(atPath: examplesPath,
                                                 withIntermediateDirectories: true,
                                                 attributes: nil)
        for fileName in fileNames {
            let data = Bundle.main.dataFromResource(fileName: fileName)
            let fullFileName = (examplesPath as NSString).appendingPathComponent(fileName)
            try? data.write(to: URL(fileURLWithPath: fullFileName))
        }
    }
}
