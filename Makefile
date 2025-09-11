EMACS ?= emacs


optional/optional.html: optional/optional.org
	$(EMACS) --init-directory=emacs.d/ --batch --load emacs.d/init.el  --load etc/bbg-footer.el -f package-initialize --eval '(setq enable-local-variables :all)'  --visit optional/optional.org --eval '(org-transclusion-mode t)' -f org-re-reveal-export-to-html

.PHONY: slides
slides: optional/optional.html

.PHONY: elpa
elpa:
	$(EMACS) --init-directory=emacs.d/ --batch --load emacs.d/init.el


.PHONY: refresh
refresh:
	$(EMACS) --init-directory=emacs.d/ --batch --load emacs.d/init.el -f package-upgrade-all

.PHONY: clean
clean:
	rm optional/optional.html
