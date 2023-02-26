.PHONY: clean All

All:
	@echo "----------Building project:[ unaligned - Debug ]----------"
	@cd "unaligned" && "$(MAKE)" -f  "unaligned.mk" && "$(MAKE)" -f  "unaligned.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ unaligned - Debug ]----------"
	@cd "unaligned" && "$(MAKE)" -f  "unaligned.mk" clean
