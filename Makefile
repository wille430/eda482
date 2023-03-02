.PHONY: clean All

All:
	@echo "----------Building project:[ test_malloc - Debug ]----------"
	@cd "test_malloc" && "$(MAKE)" -f  "test_malloc.mk" && "$(MAKE)" -f  "test_malloc.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ test_malloc - Debug ]----------"
	@cd "test_malloc" && "$(MAKE)" -f  "test_malloc.mk" clean
