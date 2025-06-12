;;; -*- Mode: Lisp -*-
;;;
;;; Gnu Emacs initialization file (manta incarnation)

;;; General initializations
;;;
;;
;; on mac
;; size 16
;; Lucida   Lexi Mono
;;
;;Sun Oct 10 17:25:18 EDT 2021
;; stuff about cursor, which is good on Mac but huge on emacs from sec
;; checked  cursor-type    x-pointer-shape
;;   https://emacs.stackexchange.com/questions/14274/how-do-you-change-the-text-cursor-height-and-thickness





(add-hook 'Perl-mode-hook (lambda () (font-lock-mode -1)))
(add-hook 'perl-mode-hook (lambda () (font-lock-mode -1)))
                                        ;
; test
;(setq isearch-slow-terminal-mode t)
;(setq search-slow-window-lines 12)
;;;;

(menu-bar-mode -1)

;; Added by Package.el.  This must come before configurations of
;; installed packages.  Don't delete this line.  If you don't want it,
;; just comment it out by adding a semicolon to the start of the line.
;; You may delete these explanatory comments.
(package-initialize)

(tool-bar-mode -1)
;(scroll-bar-mode -1)
;(tooltip-mode -1)
;X(transient-mark-mode -1)


;(normal-erase-is-backspace-mode 1)

(defun makesig (&optional arg)
  "I am quite at a loss"
  (interactive "P")


 (insert (format "Best regards,\n\n\nBrendan\n\n")

    
);end if 
 (backward-char 1)

)
;end defun

(global-set-key "\C-xs" 'makesig)


;
; Stuff added for BSDI 4.3 to make fakemail work/ignore Wed Oct  2 10:28:11 EDT 2002
;

;;(setq send-mail-function 'smtpmail-send-it)
;;(setq smtpmail-default-smtp-server "sec.com")
;;(setq smtpmail-smtp-service "smtp")
;;(setq smtpmail-local-domain "sec.com")
;;(setq smtpmail-debug-info t) 
;;;(setq smtpmail-debug-info 'nil) ;; if no debug info needed
;;(load-library "smtpmail")
;;(setq smtpmail-code-conv-from nil)
;;(setq user-full-name "G B Reilly")

;;     (setq auto-mode-alist (cons '("\\Rakefile\\'" . ruby-mode) auto-mode-alist))
     (setq auto-mode-alist (cons '("\\.make\\'" . makefile-mode) auto-mode-alist))
     (setq auto-mode-alist (cons '("\\.inl\\'" . c++-mode) auto-mode-alist))
     (setq auto-mode-alist (cons '("\\.in\\'" . c++-mode) auto-mode-alist))
     (setq auto-mode-alist (cons '("\\.pro\\'" . c++-mode) auto-mode-alist))
     (setq auto-mode-alist (cons '("\\.emacs\\'" . emacs-lisp-mode) auto-mode-alist))


;      (autoload 'php-mode "/u/reilly/emacs/php-mode.elc" "PHP editing mode" t)
;      (add-to-list 'auto-mode-alist '("\\.php3\\'" . php-mode))
;      (add-to-list 'auto-mode-alist '("\\.php4\\'" . php-mode))
;      (add-to-list 'auto-mode-alist '("\\.php\\'" . php-mode))
;
; For sunrise/sunset
;
;
(setq calendar-location-name "Greenwich,CT")
;     (setq calendar-latitude 39.945387)
;     (setq calendar-longitude -75.153405)


;     (setq calendar-location-name "San Antonio, TX")
;     (setq calendar-latitude 29.53)
;     (setq calendar-longitude -98.47)




;     (setq calendar-time-zone -480)
     (setq calendar-standard-time-zone-name "EST")
     (setq calendar-daylight-time-zone-name "EDT")



(defun makecaleasy (&optional arg)
  "I am quite at a loss"
  (interactive "P")

; Current time returns three numbers

;
;  (if arg (insert (format "%d" arg))

(if arg (insert (format-time-string "%b %d, %Y :" 
  (cons (+ 1 (car (current-time))) (cdr (current-time)))))

;else

    (insert  (format-time-string "%b %d, %Y :" (current-time)))

				;end else
    
    );end if 
 (backward-char 1)
);end defun


(defun makecaleasy00 (&optional arg)
  "I am quite at a loss"
  (interactive)
(beginning-of-line 1)
(open-line 1)
(insert (current-time-string))
(backward-word 1)
(backward-kill-word 3)
(backward-char 1)
(insert ",")
(backward-word 2)
(backward-kill-word 1)
(end-of-line)
(insert " : ")
(backward-char 2)

)

(global-set-key "\C-x\C-d" 'makecaleasy)



;;;Always search using insensitivity to case
(setq-default case-fold-search t)
(setq-default sort-fold-case t)

;;; if available make a mark when marking region
;;;(setq-default transient-mark-mode t)


(setq inhibit-startup-message t)	; skip the startup message



;1 (setq inhibit-local-variables t)	; act on local variables
;1 (setq enable-local-variables t)
(setq default-major-mode 'fundamental-mode)	; Text Mode is the default
;(setq text-mode-hook			; Auto-Fill on in Text Mode buffers
;      '(lambda nil (auto-fill-mode 1)
;	 (setq fill-column 63)			; fill column is 55
;           (or
;	    (string-match '"calendar" filename)
;	       (progn (setq fill-column 9999999)) ; fill column is infinite
;
;	     )

;)
;)

(setq indent-tabs-mode nil)
(setq-default indent-tabs-mode nil)



;;;(setq c-mode-common-hook
;;;      '(lambda nil (setq comment-column 48)
;;;         (abbrev-mode -1)
;;;	           (modify-syntax-entry 43 "w")
;;;	           (modify-syntax-entry 95 "w")
;;;		   (setq case-fold-search t)
;;;;new stuff
;;;		   (setq indent-tabs-mode nil)
;;;		   (setq c-auto-newline nil)
;;;		   (setq c-tab-always-indent nil)
;;;		   (setq c-indent-level 3)
;;;		   (setq c-continued-statement-offset 0)
;;;		   (setq c-brace-offset 0)
;;;		   (setq c-argdecl-indent 0)
;;;                   (setq c-label-offset -2)
;;;                   (global-unset-key "¿")
;;;(transient-mark-mode -1)
;;;
;;;(setq hide-ifdef-shadow t)
;;;  (setq hide-ifdef-initially t) 
;;;  (hide-ifdef-mode 1)
;;;
;;;
;;;
;;;))






(setq fill-column 65)			; fill column is 55
(put 'eval-expression 'disabled nil)
(setq auto-save-interval 10)		; 10 chars between auto-saves
(setq echo-keystrokes 1)		; wait 1 sec before echo at bottom

(setq enable-recursive-minibuffers t)	; we can handle 'em
(setq make-backup-files 'nil)		; backup files necessary
(setq backup-by-copying 'nil)
; backup-by-copying-when-linked only if above is nil
(setq backup-by-copying-when-mismatch 't)


(setq require-final-newline t)		; tack on final newlines automatically
(setq version-control t)		; don't need it

;;; put our local macro library into the load-path
;;  The line below will stop 
;;  phases of the moon from working
;;  Don't know why.  It also appeared
;;  to stop calendar to working
;;
;; mv'd to emacs_19_lib (setq load-path (append (list (expand-file-name "~/emacs")) load-path))
;(setq load-path (append (list (expand-file-name "~/Src/jde-2.0.1")) load-path))

;;; archive outgoing messages
;(setq mail-archive-file-name (expand-file-name "~/mail/archive"))
;;
;Try to not have to watch mail
(setq mail-interactive 'nil)

;;; fix location for M-x man
(setq manual-program "/usr/bin/man")
;(setq manual-formatted-dirlist
;      '("/usr/catman/man1" "/usr/catman/man2" "/usr/catman/man3"
;	"/usr/catman/man4" "/usr/catman/man5" "/usr/catman/man6"
;	"/usr/catman/man7" "/usr/catman/man8"))

;;; Global Key Bindings
;;;
;(global-set-key "\C-xr" 'rmail)


(fset 'addreply
   "\274\C-ofcc: Match\C-mFrom: reilly@sec.com\C-mReply-to: reilly@sec.com\C-[<\C-[xrepls\C-? str\C-mreilly@sec.com\C-mroon02@sec.com\C-m\C-n\C-n\C-n\C-n\C-n")

;(global-set-key "\C-x\C-r" 'addreply)

;(load "~/clsc.elc")
;(load "~/rev.elc")

; Need to eliminate the need for using ^C

(global-set-key "" 'next-error)
(global-set-key "m" 'compile)
(global-set-key "g" 'goto-line)


(put 'narrow-to-region 'disabled nil)

;(setq display-time-day-and-date t)	;show day, mon, date
(setq display-time-format "%A")
(setq display-time-mail-file (expand-file-name "~/.real_mail"))
(display-time)				;Marvin - what time is it?
(if (not (string= (getenv "HOME") "/root"))
(progn

;
; Calendar
;
;(setq  mark-diary-entries-in-calendar t)
;(setq  view-calendar-holidays-initially t)
;(setq view-calendar-holidays-initially t)
(setq view-diary-entries-initially t)
;(setq appt-display-interval 15)   ; 15 minutes btwn warnings
;(setq appt-message-warning-time 15) ; check calendar every 15 minutes
(add-hook 'list-diary-entries-hook 'sort-diary-entries)
(add-hook 'nongregorian-diary-listing-hook 'sort-diary-entries)
;;
; new from Germany
(setq all-christian-calendar-holidays t)
(setq calendar-date-display-form
      '((if dayname (concat dayname ", ")) day " " monthname " " year))
(setq calendar-time-display-form
      '(24-hours ":" minutes
                 (if time-zone " (") time-zone (if time-zone ")")))

;;
;(normal-erase-is-backspace-mode 1)  


(add-hook 'diary-display-hook 'fancy-diary-display)
;;;(add-hook 'diary-hook 'appt-make-list)
;;;(setq appt-display-duration 120)
;;;;breaks in 20.4.1   (diary 0)
))
;;end calendar 





;(global-set-key "" 'next-line)

;(setq suspend-hook			; Don't allow suspend's on SCO UNIX
;      '(lambda nil (t)))

(global-set-key "z" 'suspend-emacs)
(global-set-key "Z" 'suspend-emacs)

(global-set-key "h" 'fixup-whitespace)



;
; SEC console specialities
;

(if (string= (getenv "TERM") "ibmpc3")
    (progn
      (global-set-key "ˆ" 'backward-kill-word)
    )
)

;;; archive outgoing messages


;(load "/log/MOZART/asa.elc")
(defvar mh-progs "/usr/contrib/mh/bin"   "Directory containing MH commands")
(defvar mh-lib  "/usr/contrib/lib/mh"   "Directory containing MH commands")
;(defvar mh-scan-prog "/u/reilly/lmh" "Get just messages to me")
(defvar mh-scan-prog "/usr/contrib/mh/bin/scan" "Scan command for MH")

(defvar mh-visible-headers "^Date: \\|^To: \\|^cc: \\|^Subject\\|^From:"
  "*If non-nil, contains a regexp specifying the headers to keep when cleaning.
Only used if `mh-clean-message-header' is non-nil.  Setting this variable
overrides `mh-invisible-headers'.")


(setq mh-repl-formfile "/u/reilly/Mail/replcomps")
(setq mh-ins-buf-prefix "	")
(setq mh-summary-height 6)
(setq mh-recenter-summary-p t)
(setq mh-clean-message-header t)







(put 'downcase-region 'disabled nil)

;(if (file-exists-p "private.el")
;;    (progn
;;      (load-file "private.el")
;;    )
;;)
;;
;;(setq find-file-hooks			; Auto-Fill on in Text Mode buffers
;;      '(lambda nil 
;;	    (if (file-exists-p "private.el")
;;		(progn
;;		  (load-file "private.el")
;;		)
;;	    )
;;))



; (autoload 'html-mode "html-mode" "HTML major mode." t)
; (or (assoc "\\.html$" auto-mode-alist)
;   (setq auto-mode-alist (cons '("\\.html$" . html-mode) 
;                               auto-mode-alist)))



(put 'upcase-region 'disabled nil)

(global-set-key "Ý" 'forward-paragraph)
(global-set-key "Û" 'backward-paragraph)

; For Mac
;      (global-set-key "" 'backward-kill-word)






(global-unset-key "")
; (set-frame-font "9x15")  ; use this for Excel at 636
; (set-default-font "-adobe-courier-bold-r-normal--24-240-75-75-m-150-iso8859-1")
;X    (set-foreground-color "DarkBlue")
;X    (set-background-color "DarkGray")
;X    (set-cursor-color "Red")
;X    (set-mouse-color "Green")
    (setq visible-bell t)





;; This fixes backspace, but only on terminal connections
(if (not (getenv "DISPLAY") ) (normal-erase-is-backspace-mode 1) )

; (set-default-font "8x13")
;X    (set-foreground-color "DarkBlue")
;X    (set-background-color "DarkGray")
;X    (set-cursor-color "Red")
;X    (set-mouse-color "Green")
    (setq visible-bell t)

(setq grep-command "grep -d skip -I -Hni ")
;(setq grep-find-command "find `pwd` -type f -print0 | xargs -0 -e grep -n -I -i ")

;(setq blink-cursor-delay 9999999)

;(setq blink-cursor-interval 9999999)

;(setq blink-cursor nil)

;(setq blink-cursor-mode nil)

(setq visible-cursor t)
(blink-cursor-mode -1)

(global-set-key "9" 'delete-frame)


;            remember you might need 
;X(normal-erase-is-backspace-mode)
;            if on the system console


;
;  use this in converting perl to c
;


; (setq default-frame-alist (quote ((width . 80) (height . 55) (menu-bar-lines . 0))))

; (setq initial-frame-alist (quote ((width . 80) (height . 55) (menu-bar-lines . 0))))







;(setq before-make-frame-hook 'frame-start-hook)
(setq after-make-frame-functions 'frame-start-hook)

(defun frame-start-hook (&optional arg)
(frame-parameters arg)
(x-focus-frame arg)
(select-frame arg )			;cannot use this with before hook
 (set-foreground-color "#692955")
    (set-background-color "DarkGray")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)
    (set-frame-font "10x20")
     (message "10x20 345")
(set-frame-height arg 20)
(set-frame-width arg 80)
(select-frame arg )
)
;X (transient-mark-mode -1)
(global-unset-key "\C-x\C-m")
(global-unset-key "\C-xn")
;(global-set-key "\C-xn" 'compilemon)




(global-unset-key "")
(global-unset-key "")
(global-unset-key "")


;
; make all frames have just one buffer
; 
(defun onebuffframe ()
"It is early Sun morning"
(let* ((frames (minibuffer-frame-list))
	 (list frames))

    (while (consp frames) 
       (select-frame-set-input-focus (car frames))
       (delete-other-window)
;(recenter 4)
       (setq frames (cdr frames))
;       )
      )
)
)

  (defun postcompileengine (arg) "Do me" (interactive) 
    (setq foo (get-buffer "*gud-minimon*"))
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "*gud-gdb*"))
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )

    (setq foo (get-buffer "*compilation*"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "*Buffer List*"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "*Completions*"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "gtestutils.c"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "gtklabel.c"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "gmarshal"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "gclosure"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )
    (setq foo (get-buffer "gsignal"))  
    (if 
        (buffer-live-p foo)
        (kill-buffer foo)
      )




    (gdb (concat "beta gdb -q --annotate=3  " arg)) 
)


  (defun  postcompile () "Do me" (interactive) 
    ( postcompileengine "/Src/Trade/Mon/minimon"
))

;    (setq foo (get-buffer "*gud-minimon*"))
;    (if 
;        (buffer-live-p foo)
;        (kill-buffer foo)
;      )
;    (setq foo (get-buffer "*compilation*"))  
;    (if 
;        (buffer-live-p foo)
;        (kill-buffer foo)
;      )
;    (gdb "gdb -q --annotate=3  /Src/Trade/Mon/minimon") 
;    )

(global-set-key "r" 'postcompile)

(setq utf-translate-cjk-mode nil)



;(setq gdb-use-separate-io-buffer t)  ; does not play well with (setq comint-scroll-to-bottom-on-input (quote all))
;(setq gud-gdb-command-name "gdb -q --annotate=3")
;(setq gud-tooltip-mode t)
;(setq next-error-highlight 2.5)
;(setq next-error-highlightsetq -no-select 2.5)
;(setq which-function-mode t)
;(setq gdb-find-source-frame nil)

; repeat, to see if setting as hook makes it better
                                       ;end mygdbhook
(add-hook 'gdb-mode-hook 
          (lambda ()
;(setq gdb-use-separate-io-buffer t)  ; does not play well with (setq comint-scroll-to-bottom-on-input (quote all))
;(setq gud-tooltip-mode t)
;(setq next-error-highlightsetq -no-select 2.5)
;(setq which-function-mode t)
             (gdb-speedbar-auto-raise -1)
             (setq gdb-find-source-frame nil)
             (setq gdb-enable-debug t)
             (setq gdb-find-source-frame nil)
             (setq gdb-same-frame nil)
             (setq gdb-show-main nil))
;X  (transient-mark-mode -1)
)

;     (add-to-list 'default-frame-alist '((width .  80) (height . 19)))

(if (string= (getenv "DISPLAY") "localhost:10.0" )
  (progn         (set-frame-font "8x13" t)
     (message "8x13 123")
     (add-to-list 'default-frame-alist '(font . "8x13"))
    (set-frame-height (window-frame) 24)
    (set-frame-width (window-frame) 81)
)
  (progn ; (set-default-font "-apple-times-medium-r-normal--24-240-72-72-m-240-mac-roman" t)
    (set-frame-font "10x20" t)
;;this happens on MAC
         (message "apple 4566")
         ;;need to stop multibyte when pasted from buffer
;;         (setq enable-multibyte-characters nil)

;     (add-to-list 'default-frame-alist '(font . "-apple-times-medium-r-normal--24-240-72-72-m-240-mac-roman"))
;     (add-to-list 'default-frame-alist '((width .  80) (height . 19)))
     )
)

;X  (add-to-list 'default-frame-alist '(auto-raise-mode . nil))

;;(load-file "/usr/share/emacs/site-lisp/ruby-mode.el")
(load-library "compile")
(setq compile-command " make ")

(setq c-basic-offset 3)
(setq use-dialog-box nil)               ;Sat Jan 31 18:45:05 EST 2009
(setq special-display-buffer-names
           '("*info*"  "*gud-gdb*")) ;Sat Jan 31 18:45:05 EST 2009



(speedbar -1)
;X  (auto-raise-mode -1)
(setq initial-scratch-message nil)

;Fri Feb  6 17:58:24 EST 2009
; removing since did not fix problem
;;(add-hook 'shell-mode-hook           (lambda ()             (set-process-filter proc 'comint-output-filter)))
;(add-hook 'comint-mode-hook            (lambda ()(setq comint-output-filter-functions nil))) ;add-hook
;(add-hook 'compilation-mode-hook (lambda ()(setq comint-output-filter-functions nil) (setq next-error-highlight 2.5)))

;; disable color crap
(setq-default global-font-lock-mode nil)



;;;;;;;;mouse wheel crap









;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;(defun compilation-buffer-name (args args22 args33) (generate-new-buffer-name (concat "*compilation" (file-relative-name buffer-file-name)   "*") ))
;;))



;  (defun compilemon () "Do me" (interactive) (find-file "/home/reilly/Trade/Mon/monitor.cpp" 1)(compile "make monitor")(gdb "gdb -ex run monitor"))

  (defun compilemon () "Do me" (interactive) (find-file "/home/reilly/Trade/Mon/monitor.cpp" 1)(compile "make monitor &&  env LD_LIBRARY_PATH=/usr/beta/lib:/usr/local/lib:/usr/X11R7/lib/xorg/modules:/usr/lib:/usr/local/apache2/modules:/lib:/usr/X11R7/lib:/usr/lib/sse2:/usr/X11R7/lib/xorg/modules/linux:/usr/lib/qt-3.3/lib:/usr/local/ssl/lib:/usr/X11R7/lib/xorg/modules/extensions  PATH=/usr/X11R7.4/bin:/home/reilly/i686:/usr/java/jdk1.6.0_10/bin:/Pkg/apache-ant-1.7.1/bin:/home/reilly/bin_sh:/home/reilly/thunderbird:/home/reilly/firefox:/usr/X11R7/bin:/usr/local/bin:/usr/local/sbin:/usr/contrib/texmf/bin:/usr/contrib/mh/lib:/usr/contrib/mh/bin:/usr/contrib/bin:/usr/lib/qt-3.3/bin:/bin:/usr/bin:/etc:/usr/sbin:/sbin:/opt/openoffice.org3/program  ./monitor --g-fatal-warnings"))

; (shell-command make... "*monitorcomp*")

(setq ansi-color-for-comint-mode nil)

(if  (not (string= (getenv  "XTERM_VERSION" ) nil))
 (progn
(normal-erase-is-backspace-mode 1)



))


    (set-foreground-color "Wheat")
    (set-background-color "#692955")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)

(set-frame-font "9x15")
(tty-suppress-bold-inverse-default-colors t)

;; disable color crap
(setq-default global-font-lock-mode nil)
(global-font-lock-mode 0)

(set-face-foreground 'minibuffer-prompt "white")


(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(compilation-column-number ((t (:inherit font-lock-type-face :foreground "black"))))
 '(compilation-error ((t (:inherit font-lock-warning-face :foreground "Red"))))
 '(compilation-info ((((class color) (min-colors 16) (background light)) (:foreground "DarkBlue" :weight bold))))
 '(compilation-line-number ((t (:inherit font-lock-variable-name-face :foreground "black"))))
 '(compilation-warning ((((class color) (min-colors 16)) (:foreground "Red" :weight bold))))
 '(font-lock-builtin-face ((((class color) (min-colors 88) (background dark)) (:foreground "AntiqueWhite"))))
 '(font-lock-comment-delimiter-face ((t (:inherit font-lock-comment-face :foreground "magenta1"))))
 '(font-lock-comment-face ((t (:foreground "white smoke"))))
 '(font-lock-constant-face ((((class color) (min-colors 88) (background light)) (:foreground "NavyBlue"))))
 '(font-lock-doc-face ((t (:inherit font-lock-string-face :foreground "gray0"))))
 '(font-lock-function-name-face ((((class color) (min-colors 88) (background light)) (:foreground "NavyBlue"))))
 '(font-lock-negation-char-face ((t (:foreground "white"))))
 '(font-lock-preprocessor-face ((t (:inherit font-lock-builtin-face))))
 '(font-lock-regexp-grouping-backslash ((t (:inherit bold :foreground "white"))))
 '(font-lock-regexp-grouping-construct ((t (:inherit bold :foreground "white"))))
 '(font-lock-string-face ((t (:foreground "IndianRed3"))))
 '(font-lock-variable-name-face ((((class color) (min-colors 88) (background dark)) (:foreground "NavajoWhite"))))
 '(font-lock-warning-face ((t (:foreground "white")))))

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(blink-cursor-mode nil)
 '(display-time-mode t)
 '(grep-find-command
   '("find . -type f -exec grep  -niH --null -e  \\{\\} +" . 42))
 '(gud-gdb-command-name "ggdb --annotate=1")
 '(gud-gud-gdb-command-name "ggdb --fullname")
 '(ispell-program-name "/usr/bin/ispell")
 '(large-file-warning-threshold nil)
 '(package-selected-packages '(jupyter))
 '(safe-local-variable-values
   '((vc-prepare-patches-separately)
     (diff-add-log-use-relative-names . t)
     (vc-git-annotate-switches . "-w")
     (hide-ifdef-env
      '((TESTMODE . 0)
        (MONDOVIASEC . 1)
        (MONDOVIAGOOGLE . 0)
        (MONDOVIAYAHOO . 0)
        (TURNOFFGOOG . 1)
        (NOVALA . 1)
        (LOOP_NOT_IDLE . 1)
        (LOOP_NOT_IDLE_GOOGT . 1)
        (LOOP_NOT_IDLE_WORKERS . 1)
        (LOOP_NOT_IDLE_PRINTM . 1)
        (FAKEVALUE . 0)
        (TRAPS . 1)
        (BUG100 . 0)
        (BUG101 . 0)
        (BUG102 . 0)
        (BUG103 . 0)
        (FROMFILE . 1)
        (PRINTMAIN . 0)
        (GRAPHICSMODE . 1)
        (EQGLIBIPC . 1)
        (BOOST09 . 1)
        (DBUS09 . 1)))
     (hide-ifdef-env
      '((TESTMODE . 0)
        (MONDOVIASEC . 1)
        (MONDOVIAGOOGLE . 0)
        (MONDOVIAYAHOO . 0)
        (TURNOFFGOOG . 1)
        (NOVALA . 1)
        (LOOP_NOT_IDLE . 1)
        (LOOP_NOT_IDLE_GOOGT . 1)
        (LOOP_NOT_IDLE_WORKERS . 1)
        (LOOP_NOT_IDLE_PRINTM . 1)
        (FAKEVALUE . 0)
        (TRAPS . 1)
        (BUG100 . 0)
        (BUG101 . 0)
        (BUG102 . 0)
        (BUG103 . 0)
        (FROMFILE . 1)
        (PRINTMAIN . 0)
        (GRAPHICSMODE . 1)
        (BOOST09 . 1)
        (DBUS09 . 1)))
     (hide-ifdef-env
      '((TESTMODE . 0)
        (MONDOVIASEC . 1)
        (MONDOVIAGOOGLE . 0)
        (MONDOVIAYAHOO . 0)
        (TURNOFFGOOG . 1)
        (NOVALA . 1)
        (LOOP_NOT_IDLE . 1)
        (LOOP_NOT_IDLE_GOOGT . 1)
        (LOOP_NOT_IDLE_WORKERS . 1)
        (LOOP_NOT_IDLE_PRINTM . 1)
        (FAKEVALUE . 0)
        (TRAPS . 1)
        (BUG100 . 0)
        (BUG101 . 0)
        (BUG102 . 0)
        (BUG103 . 0)
        (FROMFILE . 1)
        (PRINTMAIN . 1)))
     (hide-ifdef-initially t)
     (hide-ifdef-env
      '((TESTMODE . 0)
        (MONDOVIASEC . 1)
        (MONDOVIAGOOGLE . 0)
        (MONDOVIAYAHOO . 0)
        (TURNOFFGOOG . 1)
        (NOVALA . 1)
        (LOOP_NOT_IDLE . 1)
        (LOOP_NOT_IDLE_GOOGT . 1)
        (LOOP_NOT_IDLE_WORKERS . 1)
        (LOOP_NOT_IDLE_PRINTM . 1)
        (FAKEVALUE . 0)
        (TRAPS . 1)
        (BUG100 . 0)
        (BUG101 . 0)
        (BUG102 . 0)
        (BUG103 . 0)
        (FROMFILE . 1)
        (PRINTMAIN . 0)
        (__CHAR_BIT__ . 8)
        (__FLT_EVAL_METHOD__ . 0)
        (__FINITE_MATH_ONLY__ . 0)
        (__APPLE_CC__ . 5484)
        (__SCHAR_MAX__ . 127)
        (__STDC_HOSTED__ . 1)
        (__DBL_DIG__ . 15)
        (__strong . 1)
        (__APPLE__ . 1)
        (__DECIMAL_DIG__ . 21)
        (__LDBL_HAS_QUIET_NAN__ . 1)
        (__DYNAMIC__ . 1)
        (__GNUC__ . 4)
        (__MMX__ . 1)
        (__DBL_HAS_INFINITY__ . 1)
        (OBJC_NEW_PROPERTIES . 1)
        (__weak . 1)
        (__SSE2_MATH__ . 1)
        (__DBL_HAS_QUIET_NAN__ . 1)
        (__NO_INLINE__ . 1)
        (__i386 . 1)
        (__FLT_MANT_DIG__ . 24)
        (i386 . 1)
        (__i386__ . 1)
        (__FLT_RADIX__ . 2)
        (__SSE_MATH__ . 1)
        (__FLT_HAS_QUIET_NAN__ . 1)
        (__FLT_HAS_INFINITY__ . 1)
        (__LITTLE_ENDIAN__ . 1)
        (__LDBL_MANT_DIG__ . 64)
        (__CONSTANT_CFSTRINGS__ . 1)
        (__FLT_DIG__ . 6)
        (__FLT_MAX_EXP__ . 128)
        (__DBL_MANT_DIG__ . 53)
        (__SSE__ . 1)
        (__MACH__ . 1)
        (__GNUC_PATCHLEVEL__ . 1)
        (__LDBL_HAS_INFINITY__ . 1)
        (__PIC__ . 1)
        (__SSE2__ . 1)
        (__GNUC_MINOR__ . 0)
        (__LDBL_DIG__ . 18)
        (__GNUC_GNU_INLINE__ . 1)))))
 '(show-paren-mode t)
 '(transient-mark-mode t))




(defun uncolor-after-load (&rest foo)
  (dolist (face (face-list))
    (set-face-attribute face nil :foreground nil :background nil)))

(add-to-list 'after-load-functions #'uncolor-after-load)

(add-to-list 'package-archives '("melpa" . "https://melpa.org/packages/"))



(defvar permanently-enabled-local-variables '(lexical-binding fill-column))

(setq enable-local-variables t)
(setq enable-recursive-minibuffers t)
(setq hack-local-variables t)
(setq local-enable-local-variables 't)

(setq enable-local-eval t)
(setq enable-local-variables :all)

;;stop deleting a region with backspace/del
(setq delete-active-region 'nil)

;(set-frame-height arg 14)
;(set-frame-width arg 80)
;(set-frame-height (window-frame) 24)
;(set-frame-width (window-frame) 81)
