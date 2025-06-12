;;; -*- Mode: Lisp -*- sec  thurs
;;;
;;; Gnu Emacs initialization file (manta incarnation)

;;; General initializations
;;;
;

;; fix email buffers that are multibyte
;; https://emacs.stackexchange.com/questions/5732/how-to-strip-invalid-utf-8-characters-from-a-string  


;to start saving sessions
; https://ftp.gnu.org/old-gnu/Manuals/emacs-20.7/html_node/emacs_424.html

;;stop deleting a region with backspace/del
(setq delete-active-region 'nil)

(advice-add 'risky-local-variable-p :override #'ignore)

;;tired of C-Xp
(global-set-key "\C-xp" 'other-window)


;;url space remover for linux
;;(fset 'urlspace   [?\M-< escape ?% return ?! ?\M-< ?\M-% ?  return return ?!])
(fset 'urlspace   [?\M-< escape ?% return ?! ?\M-< ?\M-% ?  return return ])
(global-set-key "\C-cs" 'urlspace)
(global-unset-key "\M-j")

;;(fset 'fix23
;;   (kmacro-lambda-form [?\C-e ?\C-b ?\C-@ ?\C-a ?\C-w ?\C-? ?\C-q ?\C-i ?\C-e ?\C-f] 0 "%d"))
;;(global-set-key "n" 'fix23)

;;Thu Feb 22 11:20:50 EST 2024
(setq abbrev-suggest 't)




; 8 jul 2020
; should probably be in a file Mode thingie
;;(load "preview-latex.el" nil t t)
;info preview-latex

; Sat Mar  4 22:52:52 EST 2017
; try to make window comparisons ignore whitespace
(setq compare-ignore-whitespace t)


(message "%s"  "e0")

(setq inhibit-startup-message t)

;(setq initial-scratch-message nil)    Fri May 22 20:32:10 EDT 2015   taken out due to error message


(setq startup-screen-inhibit-startup-screen t)

(setq debug-on-error t)


(setq baud-rate 9600)


;(setq amislow (substring (getenv "REMOTEHOST") 0 4) )

; (if (or
;      (string= (substring (getenv "REMOTEHOST") 0 4) "170.") 
;      (string= (substring (getenv "REMOTEHOST") -1 9) "myvzw.com")
;      )
;(progn     
;					; test
;     (setq isearch-slow-terminal-mode t)
;   (setq search-slow-window-lines 1)
;   (setq i-am-slow 1)
;)					;end progn
;)					;end if

;;;;Sun Feb 17 15:21:03 EST 2013
;;     Menu buffer to show you what files it has visited.  You can
;;     inhibit that by setting `inhibit-startup-buffer-menu' to `t'.

(setq inhibit-startup-buffer-menu t)


;;;;

;Tue Oct  4 09:21:09 EDT 2005
; testing seeing compilation output live
(require 'compile)
(setq compilation-scroll-output t)
(setq compile-command "make ")



(menu-bar-mode -1)
(tool-bar-mode -1)
(scroll-bar-mode -1)
(tooltip-mode -1)


;
; Stuff added for BSDI 4.3 to make fakemail work/ignore Wed Oct  2 10:28:11 EDT 2002
;



(setq send-mail-function 'smtpmail-send-it)
(setq smtpmail-default-smtp-server "sec.com")
(setq smtpmail-smtp-service "smtp")
(setq smtpmail-local-domain "sec.com")
(setq smtpmail-debug-info t) 
;(setq smtpmail-debug-info 'nil) ;; if no debug info needed
(load-library "smtpmail")
(setq smtpmail-code-conv-from nil)
(setq user-full-name "G B Reilly")




(defvar interpreter-mode-alist
  '(("perl" . perl-mode)
    ("perl5" . perl-mode)
    (".cshrc" . shell-script-mode)
    (".login" . shell-script-mode)
    ("miniperl" . perl-mode)
    ("wish" . tcl-mode)
    ("config.*" . shell-script-mode)
    (".*config" . shell-script-mode)
    ("wishx" . tcl-mode)
    ("tcl" . tcl-mode)
    ("tclsh" . tcl-mode)
    ("awk" . awk-mode)
    ("mawk" . awk-mode)
    ("nawk" . awk-mode)
    ("gawk" . awk-mode)
    ("scm" . scheme-mode)
    ("ash" . sh-mode)
    ("bash" . sh-mode)
    ("bash2" . sh-mode)
    ("csh" . sh-mode)
    ("conf" . sh-mode)
    ("dtksh" . sh-mode)
    ("es" . sh-mode)
    ("itcsh" . sh-mode)
    ("jsh" . sh-mode)
    ("ksh" . sh-mode)
    ("oash" . sh-mode)
    ("pdksh" . sh-mode)
    ("rc" . sh-mode)
    ("rpm" . sh-mode)
    ("sh" . sh-mode)
    ("sh5" . sh-mode)
    ("tcsh" . sh-mode)
    ("wksh" . sh-mode)
    ("wsh" . sh-mode)
    ("zsh" . sh-mode)
    ("tail" . text-mode)
    ("more" . text-mode)
    ("less" . text-mode)
    ("alias" . alias-mode)
    ("cf" . makefile-mode)
    ("defs" . makefile-mode)
    ("pg" . text-mode))
  "Alist mapping interpreter names to major modes.
This alist applies to files whose first line starts with `#!'.
Each element looks like (INTERPRETER . MODE).
The car of each element is compared with
the name of the interpreter specified in the first line.
If it matches, mode MODE is selected.")

(autoload 'alias-mode "/u/reilly/emacs/alias-mode.el" "ALIAS major mode." t)

(autoload 'changed-mode "/u/reilly/emacs/changed-html.el" "CHANGED major mode." t)

(defvar interpreter-mode-alist
  '(("perl" . perl-mode)
    ("perl5" . perl-mode)
    ("miniperl" . perl-mode)
    ("wish" . tcl-mode)
    ("wishx" . tcl-mode)
    ("config.*" . shell-script-mode)
    (".*config" . shell-script-mode)
    ("tcl" . tcl-mode)
    ("tclsh" . tcl-mode)
    ("awk" . awk-mode)
    ("mawk" . awk-mode)
    ("nawk" . awk-mode)
    ("gawk" . awk-mode)
    ("scm" . scheme-mode)
    ("ash" . sh-mode)
    ("bash" . sh-mode)
    ("bash2" . sh-mode)
    ("csh" . sh-mode)
    ("conf" . sh-mode)
    ("dtksh" . sh-mode)
    ("es" . sh-mode)
    ("itcsh" . sh-mode)
    ("jsh" . sh-mode)
    ("ksh" . sh-mode)
    ("oash" . sh-mode)
    ("pdksh" . sh-mode)
    ("rc" . sh-mode)
    ("rpm" . sh-mode)
    ("sh" . sh-mode)
    ("sh5" . sh-mode)
    ("tcsh" . sh-mode)
    ("wksh" . sh-mode)
    ("wsh" . sh-mode)
    ("zsh" . sh-mode)
    ("tail" . text-mode)
    ("more" . text-mode)
    ("less" . text-mode)
    ("html" . changed-mode)
    ("pg" . text-mode))
  "Alist mapping interpreter names to major modes.
This alist applies to files whose first line starts with `#!'.
Each element looks like (INTERPRETER . MODE).
The car of each element is compared with
the name of the interpreter specified in the first line.
If it matches, mode MODE is selected.")




;      (autoload 'php-mode "/u/reilly/emacs/php-mode" "PHP editing mode" t)
;      (add-to-list 'auto-mode-alist '("\\.php3\\'" . php-mode))
;      (add-to-list 'auto-mode-alist '("\\.php4\\'" . php-mode))
;      (add-to-list 'auto-mode-alist '("\\.php\\'" . php-mode))
;
; For sunrise/sunset
;
;
;https://www.google.com/maps/place/353+Lexington+Ave,+New+York,+NY+10016/@40.7498682,-73.9768714,18.95z/data=!4m2!3m1!1s0x89c25903e243cf89:0xde4834472507557f

(setq calendar-location-name "SEC, NY")
     (setq calendar-latitude 40.756654)
     (setq calendar-longitude -73.971685)


;(setq calendar-location-name "Natick")
;     (setq calendar-latitude 42.34560)
;     (setq calendar-longitude -71.35792)



;(setq calendar-location-name "Rivers Abode, CT")
;     (setq calendar-latitude 41.05893)
;     (setq calendar-longitude -73.56852)

;(setq calendar-location-name "The Park, NY")
;     (setq calendar-latitude 40.7464)
;     (setq calendar-longitude -73.910869)

;phila
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

(defun makesig (&optional arg)
  "I am quite at a loss"
  (interactive "P")


 (insert (format "Best regards,\n\n\nBrendan\n\n")

    
);end if 
 (backward-char 1)

)
;end defun




(defun makesigw (&optional arg)
  "I am quite at a loss"
  (interactive "P")


 (insert (format "Warmest regards,\n\n\nBrendan\n\n")

    
);end if 
 (backward-char 1)

)
;end defun

;;fix to put ending date using --date="next hour"
(fset 'dateStamp2
   [?\[ ?s ?t ?a ?r ?t ?  ?\C-u escape ?! ?d ?a ?t ?e return ?\C-e ?\] return return return return return ?\[ ?e ?n ?d ?  ?\C-u escape ?! ?d ?a ?t ?e ?  ?- ?- ?d ?a ?t ?e ?= ?\" ?n ?e ?x ?t ?  ?h ?o ?u ?r ?\" return ?\C-e ?\] ?\C-r ?s ?t ?a ?r ?\C-n])





(fset 'dateStamp
   [?\C-a ?\C-o ?\C-o ?\[ ?\C-u ?\M-! ?d ?a ?t ?e return ?\C-a ?\C-k ?\C-y ?\] ?\C-a ?\C-k ?\C-k ?\C-k ?\C-y ?\C-u ?\C-y ?\C-f ?e ?n ?d ?  ?\C-a ?\C-o ?\C-o ?\C-p ?\C-p ?\C-n])

(fset 'bracketspeaker
   "\C-a\C-kstsart\C-?\C-?\C-?\C-?\C-?\C-?[start \C-y]\C-m\C-m\C-m[end \C-y]\C-a\C-p\C-p\C-m")


(setq mslog
   [?\C-u ?\M-! ?d ?a ?t ?e ?  ?+ ?\" ?% ?F ?  ?% ?R ?\" return ?\C-e ?\C-q ?\C-i])



(global-set-key "\C-x:" 'mslog)

(global-set-key "\C-xt" 'dateStamp2)
(global-set-key "\C-x%" 'bracketspeaker)




(defun makesigy (&optional arg)
  "I am quite at a loss"
  (interactive "P")


 (insert (format "XO\n\n\nBrendan\n\n")

    
);end if 
 (backward-char 1)

)
;end defun


(defun makesigt (&optional arg)
  "I am quite at a loss"
  (interactive "P")


 (insert (format "88\n\n\nBrendan\n\n")

    
);end if 
 (backward-char 1)

)
;end defun




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
(global-set-key "\C-xs" 'makesig)
(global-set-key "\C-xw" 'makesigw)
;(global-set-key "\C-xt" 'makesigt)



;;;Always search using insensitivity to case
(setq-default case-fold-search t)
(setq-default sort-fold-case t)

;;; if available make a mark when marking region
(setq-default transient-mark-mode t)


(setq inhibit-startup-message t)	; skip the startup message



;;WHAT IS THIS?  (setq inhibit-local-variables t)	; act on local variables
;;it blows that it will not stop asking
(setq enable-local-variables ":all")

(setq local-enable-local-variables 't)

(setq enable-local-eval t)



(setq enable-local-variables t)
(setq enable-recursive-minibuffers t)
(setq hack-local-variables t)




(setq default-major-mode 'text-mode)	; Text Mode is the default
(setq text-mode-hook			; Auto-Fill on in Text Mode buffers
      '(lambda nil (auto-fill-mode 0)
	 (setq fill-column 63)			; fill column is 55
;           (or
;	    (string-equal '"calendar" filename)
;	       (progn (setq fill-column 9999999)) ; fill column is infinite
;
;	     )

))
(setq c-mode-hook
      '(lambda nil (setq comment-column 48)
	           (modify-syntax-entry 43 "w")
	           (modify-syntax-entry 95 "w")
		   (setq case-fold-search t)
;new stuff
		   (setq c-auto-newline nil)
		   (setq c-tab-always-indent nil)
		   (setq c-indent-level 6)
		   (setq c-continued-statement-offset 0)
		   (setq c-brace-offset 0)
		   (setq c-argdecl-indent 4)
		   (setq c-label-offset -2)

;(global-set-key "control-meta-h" 'backward-kill-word)



))
(setq fill-column 55)			; fill column is 55
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



;;; On the UNIX PC, c-@ doesn't seem to make its way to Gnu Emacs.  So
;;; simply define M-<space> to do set-mark.  Also, the "delete" key is
;;; in an annoying place--swap the functionality of the delete and back
;;; space keys.


;(load "~/clsc.elc")
;(load "~/rev.elc")

; Need to eliminate the need for using ^C

(global-set-key "" 'next-error)
(global-set-key "c" 'save-buffers-kill-terminal)
(global-set-key "m" 'compile)
(global-set-key "g" 'goto-line)
(global-set-key "r" 'addrev)


(put 'narrow-to-region 'disabled nil)

; fix calendar for hines
;(defun hinescal () "fix"  (progn ( insert "\\month")))
(defun hinescal (&optional arg) "fix"  (interactive "P")  (progn ( insert (format-time-string "%Y-%h-%d %H:%M:%S" (current-time))) (insert "\t")))
;(current-time)   from time.el.gz
;%Y-%h-%d %H:%M:%S
;(fset 'hineskbd   "\C-[x\C-[OA\C-[OA\C-m\C-[OA\C-m\C-x")

(global-set-key "h" 'hinescal)


;(setq display-time-day-and-date t)	;show day, mon, date
(setq display-time-format "%a %H:%M")
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
(setq all-islamic-calendar-holidays t)
(setq all-hebrew-calendar-holidays t)
(setq calendar-date-display-form
      '((if dayname (concat dayname ", ")) day " " monthname " " year))
(setq calendar-time-display-form
      '(24-hours ":" minutes
                 (if time-zone " (") time-zone (if time-zone ")")))

;;

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

;(if (string= (getenv "TERM") "ibmpc3")
;    (progn
;      (global-set-key "?" 'backward-kill-word)
;    )
;)

;;; archive outgoing messages


;(load "/log/MOZART/asa.elc")
(defvar mh-progs "/usr/bin/mh"   "Directory containing MH commands")
(defvar mh-lib  "/usr/lib/mh"   "Directory containing MH commands")
;(defvar mh-scan-prog "/u/reilly/lmh" "Get just messages to me")
(defvar mh-scan-prog "/usr/bin/mh/scan" "Scan command for MH")

; mh-e no longer supports mh-visible-headers
;(defvar mh-visible-headers "^Date: \\|^To: \\|^cc: \\|^Subject\\|^From:"
;  "*If non-nil, contains a regexp specifying the headers to keep when cleaning.
;Only used if `mh-clean-message-header' is non-nil.  Setting this variable
;overrides `mh-invisible-headers'.")

; so add to mh-invisible-header-fields

(defvar mh-invisible-header-fields
  '(
"X-Proofpoint-Virus-Version:"
"X-Proofpoint-Spam-Details:"
"X-Enigmail-Version:"
"X-YMail-OSG:"
"X-IronPortListener:"
"X-SBRS:"
"X-IronPort-Anti-Spam-Filtered:"
"X-IronPort-Anti-Spam-Result:"
"X-AuditID:"
"User-Agent:"
"DKIM-Signature:"
"X-RedHat-Spam-Score:"
    "X-VirtualServer:"
   "X-Destination-ID:"
   "X-MailingID:"
   "X-SEF-ZeroHour-RefID:"
   "X-SMFBL:"
   "X-SMHeaderMap:"
   "X-VirtualServerGroup:"
"X-Listbox-List-ID:"
"X-Listbox-UUID:"
"X-Mailer:"
)
  "Stuff I added")







(setq mh-repl-formfile "/u/reilly/Mail/replcomps")
(setq mh-ins-buf-prefix "	")
(setq mh-summary-height 6)
(setq mh-recenter-summary-p t)
(setq mh-clean-message-header t)


(defvar my-mh-init-done nil "Non-`nil' when one-time mh-e settings made.")

(defun my-mh-folder-mode-hook ()
  "Hook to set key bindings in MH-Folder mode."
  (if (not my-mh-init-done)             ; only need to bind the keys once 
      (progn
;             (local-set-key "/" 'search-msg)
;	(local-set-key "b" 'mh-execute-commands)    ; better use of `b'
;	(local-set-key "\\\M-r" 'mh-rescan-folder)
	(setq mh-decode-mime-flag nil)
(setq mh-clean-message-header t)
(setq mh-colors-available-flag nil)

(mh-ps-print-toggle-color)
	(setq my-mh-init-done t))))

;;; Emacs 19
(add-hook 'mh-folder-mode-hook 'my-mh-folder-mode-hook)
;;; Emacs 18
;;;   (setq mh-folder-mode-hook (cons 'my-mh-folder-mode-hook
;;;                               mh-folder-mode-hook))

;;;Sat Dec  7 23:34:04 EST 2024
;;day timer table
(defun GROTmyday-watch-date (&optional arg reset)
  "hack"
  (message "1 %s" "NOT RUN myfind-watch-date")
  (message "1 in hook filename >%s<" (buffer-file-name))

   
(if (string= "/home/reilly/Day-Timer" (buffer-file-name) )

(progn 

;;  (interactive)
  (if (0)
      (

  (message "%s" "3 after end of buffer")
  (message "%s" "2 NOT RUN2 myfind-watch-date")
  (message "2 in hook filename %s" (buffer-file-name))

  (goto-char (point-max))
    (table-recognize-table)
  )
      )					;if 0

 ) 					;progn
)					;if =
)					;defun

;;;in watch.el   (add-hook 'find-file-hook  #'myday-watch-date)


;;;MH hack to change coding system to ascii
(add-to-list 'file-coding-system-alist '("\\/ssd/Mail/Draft\\'" . us-ascii))
;;;
(defun my-write-file-functions()
"hack to write in ascii"
;;could also check if Draft is part of buffer-file-name
;;this fails on the RPI machines
;;(setq coding-system-for-write 'ascii)
(message "da file %s" default-directory)
(string-equal "/Src/MISC/" default-directory)
   (message "dum file SM %s" default-directory) 
(string-equal "/Src/MISC/" default-directory)
   (setq coding-system-for-write 'ascii)

; (vc-mode) 
)

(setq find-file-hooks 'my-write-file-functions)
;;;(setq write-file-functions nil)


(defun search-msg ()
  "Search for a regexp in the current message."
  (interactive)                         ; user function
  (save-window-excursion
    (other-window 1)                    ; go to next window
    (isearch-forward-regexp)))          ; string search; hit return (ESC
					;   in Emacs 18) when done





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

;(global-set-key "?" 'forward-paragraph)
;(global-set-key "?" 'backward-paragraph)

; For Mac
;      (global-set-key "" 'backward-kill-word)


; this may have to be done as part of a hook
;(setq mh-hook '(lamba nil ( progn (

; (define-key mh-folder-mode-map "p" 'mh-previous-undeleted-msg)
; (add-hook 'mh-mode-hook 'turn-off-font-lock)

;;(setq mh-highlight-citation-style 'none)
(setq highlight-nonselected-windows t)

; ( setq mh-inc-prog "/u/reilly/bin_sh/banner_inc" )

;; for 22.3.2 this does not work, does not pull in mail
;; Nothing useful in *Messages* about why
;;(setq mh-inc-folder-hook
;; '(lambda nil  (progn ( setq mh-inc-prog "/u/reilly/bin_sh/banner_inc" ) ) ) )


;      '(lambda nil  (progn  (shell-command "lockfile -r -1 /u/reilly/LOCKS_NOT_PAGED/ATOMIC_ACTION" nil)   (if (file-exists-p "/u/reilly/.real_mail") (delete-file "/u/reilly/.real_mail")) (generate-new-buffer "dfkjrtwbr934") (shell-command "rm -f /u/reilly/LOCKS_NOT_PAGED/ATOMIC_ACTION" "dfkjrtwbr934"  "dfkjrtwbr934")  )) ) 

;
;
; Generic XTERM setup
;
;
(message "%s generic e000 %s    %s    %s" (getenv "TERM")  (string-match "localhost" (getenv "TERM") )  (frame-height)  (frame-width))
(if  (and x-initialized (string-match "localhost" (getenv "DISPLAY") ) )
(progn

  (message "%s" "resize e001")
    (set-foreground-color "Wheat")
    (set-background-color "DarkSlateBlue")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)
;    (if (< (frame-height) 6)
	(set-frame-height (window-frame) 24)
;      )
;    (if (> (frame-width) 81)
	(set-frame-width (window-frame) 81)
;      )

;;Thu 17 Mar 2022 11:01:13 AM EDT


))					;end generic xterm setup


(if (not (string-or-null-p window-system ))
(if  (and x-initialized  )   (string-match "xterm" (getenv "TERM") )
;(if  (string-match "xterm" (getenv "TERM") )
(progn

(message "%s" "fell for xterm")
    (set-foreground-color "Wheat")
    (set-background-color "DarkSlateBlue")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)


))					;end generic xterm setup
)


(if  (string-equal nil (getenv "TERM") )
(progn

(message "%s" "x2")
    (set-foreground-color "Wheat")
    (set-background-color "DarkSlateBlue")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)


))					;end generic xterm setup



(if  (string-equal "000dumb" (getenv "TERM") )
(progn

(message "%s" "x3")
    (set-foreground-color "Wheat")
    (set-background-color "DarkSlateBlue")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)
     (normal-erase-is-backspace-mode)
(message "%s"  "e1")

))					;end generic xterm setup

(if  (string-equal "dumb" (getenv "TERM") )

(message "%s"  "e2")
    (normal-erase-is-backspace-mode)
)
;
; 
; The hell of linux on a small screen at Penn
;

; (if  (string-match "130.91.181.251" (getenv "SSH_CLIENT") )    (progn (set-default-font "7x14")))


; The hell of linux on a small screen at Penn at home

; (if  (string-match "216.158.25.179" (getenv "SSH_CLIENT") )    (progn (set-default-font "8x16")))

(setq grep-command "grep -d skip -HIn  -i ")
(setq grep-find-command "find `pwd` -type f -not -path '*/\.svn'  -exec grep -HIn  -i  {}  \\;")



(global-set-key "\C-\M-h" 'backward-kill-word)

( blink-cursor-mode -1)
(setq blink-cursor-blinks 1)		;might slow down the blinking

( file-name-shadow-mode -1)

(setq ansi-color-for-comint-mode nil)
(global-hi-lock-mode 1)    ;change from -1
(global-font-lock-mode -1)
(global-unset-key "")

(icomplete-mode 1)




(if  (string-equal "xterm-color" (getenv "TERM") )
(progn
  (normal-erase-is-backspace-mode)
(message "%s"  "e3")
))





(if  (string-equal "xtermlin" (getenv "TERM") )
(progn
;;;Sat Jan 18 13:09:15 EST 2014

;; (normal-erase-is-backspace-mode)
;;(message "%s"  "e4")
))



(if  (string-equal nil (getenv "TERM") )
(progn
 (normal-erase-is-backspace-mode)
(message "%s"  "e5")

))


;;;
;; if a frame is made in X11 it never gets the correct fore/back-ground or colors
;;;
(setq after-make-frame-functions '(progn

(message "%s" "x4")

    (set-foreground-color "Wheat")
    (set-background-color "DarkSlateBlue")
    (set-cursor-color "Red")
    (set-mouse-color "Green")
    (setq visible-bell t)


))



    (setq visible-bell t)



(if  (string-equal "screen" (getenv "TERM") )
(progn
 ;;(normal-erase-is-backspace-mode)

))


;; Sat Jun 13 21:06:01 EDT 2015
;; stop mh-rmail from complaining when it does not have enough memory for undo 
(load-library "warnings")
(add-to-list 'warning-suppress-types '(undo discard-info))


;; Sat Jun 13 21:05:32 EDT 2015
;; stops some modes from splitting windows vertically, but not mh

;;only set this on X windows

;(setq split-height-threshold nil)
;(setq split-width-threshold 0)




;  (add-to-list 'load-path "/home/reilly/.emacs.d/predictive")
;  (add-to-list 'load-path "/home/reilly/.emacs.d/predictive/misc")
;  (add-to-list 'load-path "/home/reilly/.emacs.d/predictive/latex")
;  (add-to-list 'load-path "/home/reilly/.emacs.d/predictive/html")
;  (add-to-list 'load-path "/home/reilly/.emacs.d/predictive/texinfo")
;;  (require 'predictive)

;  (autoload 'predictive-mode "/path/to/predictive/predictive" "Turn on Predictive Completion Mode." t)



;; disable color crap
;;;(setq-default global-font-lock-mode nil)

(set-face-foreground 'minibuffer-prompt "white")

  (normal-erase-is-backspace-mode)

(message "%s" "xprecustom")
;;;   fix text seen on screen set-terminal-coding-system
;;; ??      default-terminal-coding-system
;; and a different thing
;;   (set-keyboard-coding-system nil)
;; list-coding-systems
;;
;;confirm-kill-emacs   set to   yes-or-no-p
(if
    (and
     (not (string-or-null-p window-system ) )
     (not (string-equal "/root/.emacs" user-init-file))
    )
(progn
 (setq confirm-kill-emacs 'yes-or-no-p)
;;;;stops emacs from exiting without confirmation


					; (set-frame-font "12x24")
  (set-frame-font "10x20")
(message "%s" "x1  font to 10x20")

))
;;(message "%s" "xexit")




(setq custom-file "~reilly/.emacs.d/custom.el")
(load custom-file)


(defun uncolor-after-load (&rest foo)
  (dolist (face (face-list))
    (set-face-attribute face nil :foreground nil :background nil)))

(add-to-list 'after-load-functions #'uncolor-after-load)

;;;mergerx support

(defvar mergerx-date-format "%Y-%m-%d"
  "Format of date to insert with `insert-current-date-time' func
See help of `format-time-string' for possible replacements")


(defun mergerx-current-date ()
  "insert the current date (1-week scope) into the current buffer."
       (interactive)
       (insert (format-time-string mergerx-date-format (current-time)))
       (insert "\n")
       )

(defun mergerx-insert-current-date ()
  "insert the current date (1-week scope) into the current buffer."
       (interactive)
       (insert (format-time-string mergerx-date-format (current-time)))
       )

(defun mergerx-finddate ()
  "insert the current date (1-week scope) into the current buffer."
       (interactive)
       ;;go to lastdate line
       (search-forward "lastrxdate")

       ;;kill line
       (move-beginning-of-line 1)
       (kill-line 1)

       ;; down line
       (next-line)
       
       ;;yank line
       (yank)

       ;;go up line
       (previous-line)

       
       (move-beginning-of-line 1)    ;;beg of line
       (search-forward ".last")


       ;;remove last
       (delete-backward-char 4)
       

       ;;change date to today
       (search-forward "'")
       (delete-forward-char 10)
       (mergerx-insert-current-date)

       ;;go up line
       (previous-line)
       (previous-line)
       

       ;;insert last

       (search-forward "med1.")
       (insert "last")


       );;;;END OF FIND       

(defun mergerx-finddate2 ()
  "insert the current date (1-week scope) into the current buffer."
       (interactive)

       



;; left ' intact       (insert ("'"))
       

       
       )
(global-set-key "\C-x'" 'mergerx-finddate)

(if (string= "/tmp/mergerx.py" (buffer-file-name) )
    (message "Use C-X'")
    )


;;;(message "current buff filename %s"   (buffer-file-name))


(defvar permanently-enabled-local-variables '(lexical-binding fill-column))

(setq enable-local-variables t)
(setq enable-recursive-minibuffers t)
(setq hack-local-variables t)
(setq local-enable-local-variables 't)

(setq enable-local-eval t)
(setq enable-local-variables :all)

;;https://emacs.stackexchange.com/questions/3673/how-to-make-vc-and-magit-treat-a-symbolic-link-to-a-real-file-in-git-repo-just
(setq find-file-visit-truename t)


;;;;START WATCH SUPPORT
;;;(add-to-list 'load-path "/home/reilly/elisp")
;;;(load "watch")
;;;;END WATCH SUPPORT
