//
//  ViewController.swift
//  splash
//
//  Created by Gonzo Fialho on 02/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class ViewController: UIDocumentBrowserViewController {

    let browserDelegate = BrowserDelegate()  // swiftlint:disable:this weak_delegate
    var transitioningController: UIDocumentBrowserTransitionController?

    init() {
        super.init(forOpeningFilesWithContentTypes: ["ninja.gonzo.splash.script"])
        delegate = browserDelegate
        let button = UIBarButtonItem(title: "Help",
                                     style: .plain,
                                     target: self,
                                     action: #selector(presentInfo(sender:)))
        additionalLeadingNavigationBarButtonItems.append(button)
    }

    required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)

        if UserDefaults.standard.alreadyShowedOnboard1 == false {
            present(OnboardViewController(), animated: animated)
            showExamplesInRecents()
        }
    }

    func presentEditor(withURL url: URL) {
        let editorViewController = EditorNavigationController()
        let document = SplashDocument(fileURL: url)

        editorViewController.transitioningDelegate = self
        let transitioningController = transitionController(forDocumentAt: url)
        transitioningController.targetView = editorViewController.view

        self.transitioningController = transitioningController

        editorViewController.set(document) {
            self.present(editorViewController, animated: true, completion: nil)
        }
    }

    fileprivate func showExamplesInRecents(_ fileNames: [String]? = nil) {
        let fileNames = fileNames ?? ["Age", "Leap Year", "Quadratic Solver"].map {$0 + ".splash"}
        guard let fileName = fileNames.first else {return}
        let remainingFiles = Array(fileNames.dropFirst())
        let documentsPath = FileManager.documentsDirectory.path
        let examplesPath = (documentsPath as NSString).appendingPathComponent("Examples")
        let fullFileName = (examplesPath as NSString).appendingPathComponent(fileName)
        let url = URL(fileURLWithPath: fullFileName)
        revealDocument(at: url,
                       importIfNeeded: false) { (_, _) in
                        self.showExamplesInRecents(remainingFiles)
        }
    }

    // MARK: - User Interaction

    @objc func presentInfo(sender: UIBarButtonItem?) {
        self.present(HelpNavigationController(), animated: true, completion: nil)
    }
}

extension ViewController: UIViewControllerTransitioningDelegate {
    func animationController(forPresented presented: UIViewController,
                             presenting: UIViewController,
                             source: UIViewController) -> UIViewControllerAnimatedTransitioning? {
        return transitioningController!
    }

    func animationController(forDismissed dismissed: UIViewController) -> UIViewControllerAnimatedTransitioning? {
        return transitioningController!
    }
}
