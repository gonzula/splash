//
//  UserDefaults+Key.swift
//  splash
//
//  Created by Gonzo Fialho on 12/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import Foundation

public extension UserDefaults {
    struct Domain: Equatable {
        public let name: String

        public init(name: String) {
            self.name = name
        }
    }

    struct Key: Equatable {
        public let domain: Domain
        public let keyName: String

        public var rawValue: String {
            return "(\(domain.name)).(\(keyName))"
        }

        public init(domain: Domain, keyName: String) {
            self.domain = domain
            self.keyName = keyName
        }
    }

    func object(forKey key: Key) -> Any? {
        return object(forKey: key.rawValue)
    }

    func url(forKey key: Key) -> URL? {
        return url(forKey: key.rawValue)
    }

    func array(forKey key: Key) -> [Any]? {
        return array(forKey: key.rawValue)
    }

    func dictionary(forKey key: Key) -> [String: Any]? {
        return dictionary(forKey: key.rawValue)
    }

    func string(forKey key: Key) -> String? {
        return string(forKey: key.rawValue)
    }

    func stringArray(forKey key: Key) -> [String]? {
        return stringArray(forKey: key.rawValue)
    }

    func data(forKey key: Key) -> Data? {
        return data(forKey: key.rawValue)
    }

    func bool(forKey key: Key) -> Bool {
        return bool(forKey: key.rawValue)
    }

    func integer(forKey key: Key) -> Int {
        return integer(forKey: key.rawValue)
    }

    func float(forKey key: Key) -> Float {
        return float(forKey: key.rawValue)
    }

    func double(forKey key: Key) -> Double {
        return double(forKey: key.rawValue)
    }

    func set(_ object: Any?, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    func set(_ object: Float, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    func set(_ object: Double, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    func set(_ object: Int, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    func set(_ object: Bool, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    func set(_ object: URL?, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    func removeObject(forKey key: Key) {
        removeObject(forKey: key.rawValue)
    }
}
