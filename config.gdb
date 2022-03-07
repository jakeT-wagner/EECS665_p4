set breakpoint pending on
set confirm off
file ./cmmc
break cmmc::Err::report
commands
	where
end
break cmmc::InternalError::InternalError
commands
	where
end

define p4
  set args p4_tests/$arg0.cmmc -n
  run
end
