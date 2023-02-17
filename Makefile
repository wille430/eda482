.PHONY: clean All

All:
	@echo "----------Building project:[ spindeljakt - Debug ]----------"
	@cd "spindeljakt" && "$(MAKE)" -f  "spindeljakt.mk" && "$(MAKE)" -f  "spindeljakt.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ spindeljakt - Debug ]----------"
	@cd "spindeljakt" && "$(MAKE)" -f  "spindeljakt.mk" clean
