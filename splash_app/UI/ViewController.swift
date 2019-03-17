//
//  ViewController.swift
//  splash
//
//  Created by Gonzo Fialho on 02/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class ViewController: UIDocumentBrowserViewController {

    var transitioningController: UIDocumentBrowserTransitionController?

    init() {
        super.init(forOpeningFilesWithContentTypes: ["ninja.gonzo.splash.script"])
        delegate = self
        let button = UIBarButtonItem(title: "Help",
                                     style: .plain,
                                     target: self,
                                     action: #selector(presentInfo(sender:)))
        additionalLeadingNavigationBarButtonItems.append(button)
    }

    required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

    fileprivate func presentEditor(withURL url: URL) {
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

    fileprivate func createNewFile(named name: String) -> URL? {
        let tempDirectoryPath = NSTemporaryDirectory()
        var name = name.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !name.isEmpty else {return nil}
        if !name.lowercased().hasSuffix(".splash") {
            name += ".splash"
        }
        let filePath = (tempDirectoryPath as NSString).appendingPathComponent("\(name)")
        let url = URL(fileURLWithPath: filePath)
        do {
            try Data().write(to: url)
            return url
        } catch {
            return nil
        }
    }

    // MARK: - User Interaction

    @objc func presentInfo(sender: UIBarButtonItem?) {
        self.present(HelpNavigationController(), animated: true, completion: nil)
    }
}

extension ViewController: UIDocumentBrowserViewControllerDelegate {
    func documentBrowser(_ controller: UIDocumentBrowserViewController, didPickDocumentURLs documentURLs: [URL]) {
        guard let url = documentURLs.first else {return}

        presentEditor(withURL: url)
    }

    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         didRequestDocumentCreationWithHandler importHandler:
        @escaping (URL?, UIDocumentBrowserViewController.ImportMode) -> Void) {

        let alertController = UIAlertController(title: "New Script",
                                           message: nil,
                                           preferredStyle: .alert)
        alertController.addTextField { textField in
            textField.autocapitalizationType = .none
            textField.returnKeyType = .done
            textField.autocorrectionType = .no
            textField.spellCheckingType = .no
            textField.smartDashesType = .no
            textField.smartQuotesType = .no
            textField.placeholder = "File name"
        }
        alertController.addAction(
            UIAlertAction(title: "Create",
                          style: .default,
                          handler: { _ in
                            if let url = self.createNewFile(named: alertController.textFields?.first?.text ?? "") {
                                importHandler(url, .move)
                            } else {
                                importHandler(nil, .none)
                            }
            }))
        alertController.addAction(UIAlertAction(title: "Cancel",
                                                style: .cancel,
                                                handler: { _ in
                                                    importHandler(nil, .none)
        }))
        self.present(alertController, animated: true, completion: nil)
    }

    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         didImportDocumentAt sourceURL: URL,
                         toDestinationURL destinationURL: URL) {
        presentEditor(withURL: destinationURL)
    }

    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         failedToImportDocumentAt documentURL: URL,
                         error: Error?) {
        guard let error = error else {return}
        self.present(UIAlertController(error: error),
                     animated: true,
                     completion: nil)
    }

    /// Implement this to customize the UIActivityViewController before it's presented.
    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         willPresent activityViewController: UIActivityViewController) {

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
