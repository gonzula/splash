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

    var mainViewController = ViewController()

    func application(_ application: UIApplication,
                     didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {

        if UserDefaults.standard.alreadyInstalledExamples1 == false {
            FileManager.createExamplesDirectory()
            UserDefaults.standard.alreadyInstalledExamples1 = true
        }

        window = UIWindow(frame: UIScreen.main.bounds)
        window!.rootViewController = mainViewController
        window!.makeKeyAndVisible()

        window!.tintColor = ThemeManager.shared.theme.tintColor

        return true
    }

    func application(_ app: UIApplication,
                     open url: URL,
                     options: [UIApplication.OpenURLOptionsKey: Any] = [:]) -> Bool {
        guard url.isFileURL else { return false }
        self.mainViewController.revealDocument(
            at: url,
            importIfNeeded: true,
            completion: {[unowned self] (url, error) in
                if let url = url {
                    self.mainViewController.presentEditor(withURL: url)
                } else if let error = error {
                    let alertController = UIAlertController(title: "Error",
                                                            message: error.localizedDescription,
                                                            preferredStyle: .alert)
                    self.mainViewController.present(alertController, animated: true, completion: nil)
                    return
                }
        })

        return true
    }
}
