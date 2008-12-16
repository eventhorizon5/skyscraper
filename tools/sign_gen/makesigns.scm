(define (apply-makesign-effect img
                                  logo-layer
                                  bg-color
                                  text-color
				  width
				  height)
  (let* (
        (bg-layer (car (gimp-layer-new img width height RGBA-IMAGE "Background" 100 NORMAL-MODE)))
        )

    (gimp-context-push)

    (gimp-selection-none img)
    (gimp-image-add-layer img bg-layer 2)
    (gimp-context-set-foreground text-color)
    (gimp-layer-set-lock-alpha logo-layer TRUE)
    (gimp-edit-fill logo-layer FOREGROUND-FILL)
    (gimp-context-set-background bg-color)
    (gimp-edit-fill bg-layer BACKGROUND-FILL)
    (gimp-context-pop)
  )
)


(define (script-fu-makesign filename text
                               size
                               font
                               bg-color
                               text-color
			       width
			       height)
  (let* (
        (img (car (gimp-image-new width height RGB)))
        (text-layer (car (gimp-text-fontname img -1 0 0 text 10 TRUE size PIXELS font)))
	(width2 (/ width 2))
	(height2 (/ height 2))
        )

    (gimp-image-undo-disable img)
    ;center text layer
    (gimp-layer-set-offsets text-layer (- width2 (/ (car (gimp-drawable-width text-layer)) 2)) (- height2 (/ (car (gimp-drawable-height text-layer)) 2)))
    (apply-makesign-effect img text-layer bg-color text-color width height)
    (gimp-image-merge-visible-layers img 0)
    (gimp-image-undo-enable img)
    (file-jpeg-save 1 img (car (gimp-image-get-active-drawable img)) filename filename 1 0 0 0 "test" 0 1 0 1)
  )
)

