(show-paren-mode 1)
(iswitchb-mode 1)
(set-scroll-bar-mode 'left)
(blink-cursor-mode -1)

(add-to-list 'load-path (expand-file-name "~/epoxide/src"))
(autoload 'epoxide-tsg-mode "epoxide")
(add-to-list 'auto-mode-alist '("\\.tsg\\'" . epoxide-tsg-mode))
(autoload 'tramp-mininet-setup "tramp-mininet")
(eval-after-load 'tramp '(tramp-mininet-setup))

(setq tramp-mininet-m-command "/bin/sh")
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(epoxide-doubledecker-jsonclient "/home/unify/DoubleDecker-py/bin/jsonclient.py")
 '(epoxide-doubledecker-key "/etc/doubledecker/public-keys.json"))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
