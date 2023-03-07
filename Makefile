.PHONY: clean All

All:
	@echo "----------Building project:[ lab5 - Debug ]----------"
	@cd "lab5" && "$(MAKE)" -f  "lab5.mk" && "$(MAKE)" -f  "lab5.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ lab5 - Debug ]----------"
	@cd "lab5" && "$(MAKE)" -f  "lab5.mk" clean
