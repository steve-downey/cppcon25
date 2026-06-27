;;; surround.el --- Surround a region with UUID markers in comments -*- lexical-binding: t; -*-

;;; Commentary:
;;

(require 'org-id)

;;; Code:
(defvar mode-to-src-map
      '(
        (awk-mode . "awk")
        (awk-ts-mode . "awk")
        (c++-mode . "c++")
        (c++-ts-mode . "c++")
        (c-mode . "c")
        (c-ts-mode . "c")
        (cmake-mode . "cmake")
        (cmake-ts-mode . "cmake")
        (emacs-lisp-mode "elisp")
        (emacs-lisp-ts-mode "elisp")
        (haskell-mode . "haskell")
        (haskell-ts-mode . "haskell")
        (makefile-mode . "makefile")
        (makefile-ts-mode . "makefile")
        (python-mode . "python")
        (python-ts-mode . "python")
        (scala-mode . "scala")
        (scala-ts-mode . "scala")
      ))


(defvar transclude-format
  "#+transclude: [[file:%s::%s]] :lines 2- :src %s :end \"%s end\"")

(defun blank-line-p (&optional pos)
  "Returns `t' if line (optionally, line at POS) is empty or
composed only of whitespace."
  (save-excursion
    (goto-char (or pos (point)))
    (beginning-of-line)
    (= (pos-eol)
       (progn (skip-syntax-forward " ") (point)))))

(defun surround-region-with-uuids (beginning end)
  "Surround the active region with UUID in comments."
  (interactive "r")
  (comment-normalize-vars)
  (let* ((uuid (org-id-uuid))
         (name buffer-file-truename)
         (src-lang (alist-get major-mode mode-to-src-map))
         (prefix-wrap (concat comment-start uuid "\n"))
         (postfix-wrap (concat comment-start uuid " end"))
         (transclude (format transclude-format name uuid src-lang uuid))
         (b (save-excursion (goto-char (region-beginning)) (line-beginning-position)))
         (e (save-excursion (goto-char (region-end)) (line-end-position))))
    (save-restriction
      (narrow-to-region b e)
      (goto-char (point-min))
      (insert prefix-wrap)
      (goto-char (point-max))
      (if (blank-line-p)
          (beginning-of-line)
        (insert "\n"))
      (insert postfix-wrap))
    (kill-new transclude)))

(provide 'surround)

;;; surround.el ends here
