//
//  EditorViewController.swift
//  splash
//
//  Created by Gonzo Fialho on 03/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit
import StoreKit

class EditorNavigationController: UINavigationController {

    var textView: UIView {
        guard let editorViewController = viewControllers.first as? EditorViewController else {fatalError()}
        return editorViewController.textView
    }

    init() {
        super.init(rootViewController: EditorViewController())
    }

    override init(nibName nibNameOrNil: String?, bundle nibBundleOrNil: Bundle?) {
        super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
    }

    required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

    func set(_ document: SplashDocument, completion: @escaping () -> Void) {
        guard let editorViewController = viewControllers.first as? EditorViewController else {return}

        editorViewController.set(document, completion: completion)
    }
}

class EditorViewController: UIViewController {

    var observers = [Any]()

    var splashDocument: SplashDocument? {
        didSet {
            title = splashDocument?.fileName
        }
    }

    let textView = EditorView()

    override func loadView() {
        view = textView

        setupNavigationBarItems()
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        setupObservers()
        setupAppearance()
    }

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)

        textView.becomeFirstResponder()
    }

    deinit {
        observers.forEach(NotificationCenter.default.removeObserver)
    }

    private func setupAppearance() {
        let theme = ThemeManager.shared.theme
        navigationController?.navigationBar.barStyle = theme.navigationBarStyle
    }

    private func setupNavigationBarItems() {
        navigationItem.leftBarButtonItem = UIBarButtonItem(barButtonSystemItem: .done,
                                                           target: self,
                                                           action: #selector(doneTouched(sender:)))
        navigationItem.rightBarButtonItems = navigationItem.rightBarButtonItems ?? []
        navigationItem.rightBarButtonItems?.append(UIBarButtonItem(barButtonSystemItem: .play,
                                                                   target: self,
                                                                   action: #selector(compileAndRun(sender:))))
    }

    fileprivate func setupObservers() {
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(adjustForKeyboard),
                                               name: UIResponder.keyboardWillHideNotification, object: nil)
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(adjustForKeyboard),
                                               name: UIResponder.keyboardWillChangeFrameNotification, object: nil)

        observers.append(  // swiftlint:disable:next discarded_notification_center_observer
            NotificationCenter.default.addObserver(
                forName: .themeChanged,
                object: nil,
                queue: nil) { [weak self] _ in
                    self?.setupAppearance()
        })
    }

    func set(_ document: SplashDocument, completion: @escaping () -> Void) {
        self.splashDocument = document
        document.open { success in
            guard success else {return}

            self.textView.text = document.string
            self.textView.colorizeText()

            completion()
        }
    }

    func presentShortcutsNotFoundAlert() {
        let alertController = UIAlertController(title: "Error",
                                                message: "Shortcuts app not found. Install it and try again.",
                                                preferredStyle: .alert)
        alertController.addAction(UIAlertAction(title: "Ok",
                                                style: .cancel,
                                                handler: nil))
        alertController.addAction(UIAlertAction(title: "Install it",
                                                style: .default,
                                                handler: { [unowned self] _ in
                                                    self.presentAppStore()
        }))
        self.present(alertController, animated: true, completion: nil)
    }

    func presentAppStore() {
        let controller = SKStoreProductViewController()
        controller.loadProduct(withParameters: [
            SKStoreProductParameterITunesItemIdentifier: NSNumber(value: 915249334)]) { [weak self] (_, error) in
                guard let self = self else {return}
                if let error = error {
                    self.present(UIAlertController(error: error),
                                 animated: true,
                                 completion: nil)
                }
                self.present(controller, animated: true, completion: nil)
        }
        controller.delegate = self
    }

    // MARK: - User Interaction

    @objc func doneTouched(sender: UIBarButtonItem) {
        guard let document = splashDocument else {
            dismiss(animated: true, completion: nil)
            return
        }
        document.string = (textView.text ?? "")
        document.save(to: document.fileURL,
                      for: .forOverwriting) { (completed) in
                        if completed {
                            document.close { success in
                                if success {
                                    self.dismiss(animated: true, completion: nil)
                                }
                            }
                        }
        }
    }

    @objc func compileAndRun(sender: UIBarButtonItem?) {
        guard let document = splashDocument else {return}
        document.string = textView.text ?? ""
        document.compileAndRun { [unowned self] (error) in
            if let error = error {
                if error == .shortcutsNotFound {
                    self.presentShortcutsNotFoundAlert()
                } else {
                    self.present(UIAlertController(error: error), animated: true, completion: nil)
                }
            }
        }
    }
}

extension EditorViewController {
    @objc func adjustForKeyboard(notification: Notification) {
        let userInfo = notification.userInfo!

        guard let keyboardScreenEndFrame = (userInfo[UIResponder.keyboardFrameEndUserInfoKey] as? NSValue)?
            .cgRectValue else { return }
        let keyboardViewEndFrame = view.convert(keyboardScreenEndFrame, from: view.window)

        if notification.name == UIResponder.keyboardWillHideNotification {
            textView.contentInset.bottom = 0
            textView.scrollIndicatorInsets.bottom = textView.contentInset.bottom
        } else {
            let height = keyboardViewEndFrame.height
            textView.contentInset.bottom = height - view.safeAreaInsets.bottom
            textView.scrollIndicatorInsets.bottom = textView.contentInset.bottom
        }
    }
}

extension EditorViewController: SKStoreProductViewControllerDelegate {
    func productViewControllerDidFinish(_ viewController: SKStoreProductViewController) {
        viewController.dismiss(animated: true) {
            self.compileAndRun(sender: nil)
        }
    }
}
