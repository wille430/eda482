.PHONY: clean All

All:
	@echo "----------Building project:[ getsetpsr - Debug ]----------"
	@cd "getsetpsr" && "$(MAKE)" -f  "getsetpsr.mk" && "$(MAKE)" -f  "getsetpsr.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ getsetpsr - Debug ]----------"
	@cd "getsetpsr" && "$(MAKE)" -f  "getsetpsr.mk" clean
