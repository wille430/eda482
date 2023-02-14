.PHONY: clean All

All:
	@echo "----------Building project:[ movepong - Debug ]----------"
	@cd "movepong" && "$(MAKE)" -f  "movepong.mk" && "$(MAKE)" -f  "movepong.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ movepong - Debug ]----------"
	@cd "movepong" && "$(MAKE)" -f  "movepong.mk" clean
