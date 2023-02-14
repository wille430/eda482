.PHONY: clean All

All:
<<<<<<< HEAD
	@echo "----------Building project:[ movepong - Debug ]----------"
	@cd "movepong" && "$(MAKE)" -f  "movepong.mk" && "$(MAKE)" -f  "movepong.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ movepong - Debug ]----------"
	@cd "movepong" && "$(MAKE)" -f  "movepong.mk" clean
=======
	@echo "----------Building project:[ asciidisplay - Debug ]----------"
	@cd "asciidisplay" && "$(MAKE)" -f  "asciidisplay.mk" && "$(MAKE)" -f  "asciidisplay.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ asciidisplay - Debug ]----------"
	@cd "asciidisplay" && "$(MAKE)" -f  "asciidisplay.mk" clean
>>>>>>> ad55f1037f59733685b92e2023453e397c081afc
