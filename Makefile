.PHONY: clean All

All:
	@echo "----------Building project:[ random_number - Debug ]----------"
	@cd "random_number" && "$(MAKE)" -f  "random_number.mk" && "$(MAKE)" -f  "random_number.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ random_number - Debug ]----------"
	@cd "random_number" && "$(MAKE)" -f  "random_number.mk" clean
