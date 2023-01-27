.PHONY: clean All

All:
	@echo "----------Building project:[ keyb - Debug ]----------"
	@cd "keyb" && "$(MAKE)" -f  "keyb.mk" && "$(MAKE)" -f  "keyb.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ keyb - Debug ]----------"
	@cd "keyb" && "$(MAKE)" -f  "keyb.mk" clean
