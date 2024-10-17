use std::ffi::{CStr, CString};
use std::io;
use std::os::raw::c_char;
use std::process::{Command, Output};

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[no_mangle]
pub extern "C" fn execute_command(command: *const c_char) -> *mut c_char {
    // Convert the C string to a Rust string
    let c_str = unsafe {
        assert!(!command.is_null());
        CStr::from_ptr(command)
    };
    let command_str = c_str.to_str().unwrap_or("");

    // Execute the command
    let output = match run_command(command_str) {
        Ok(output) => output,
        Err(_) => "Error executing command".to_string(),
    };

    // Convert the Rust string to a C string
    let c_string = CString::new(output).unwrap();
    c_string.into_raw()
}

fn run_command(command: &str) -> Result<String, io::Error> {
    let mut parts = command.split_whitespace();
    let cmd = match parts.next() {
        Some(c) => c,
        None => {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "No command provided",
            ))
        }
    };
    let args: Vec<&str> = parts.collect();

    let output: Output = Command::new(cmd).args(&args).output()?;

    let stdout = String::from_utf8_lossy(&output.stdout).to_string();
    let stderr = String::from_utf8_lossy(&output.stderr).to_string();
    let result = format!("Stdout:\n{}\nStderr:\n{}", stdout, stderr);

    Ok(result)
}

#[no_mangle]
pub extern "C" fn free_rust_string(s: *mut c_char) {
    if s.is_null() { return; }
    unsafe {
        let _ = CString::from_raw(s); // Explicitly ignore the resulting value
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }

    #[test]
    fn test_execute_command_ls() {
        let command = CString::new("ls").unwrap();
        let result_ptr = execute_command(command.as_ptr());
        let result = unsafe { CStr::from_ptr(result_ptr).to_string_lossy().into_owned() };
        assert!(result.contains("Stdout:\n"));
        assert!(result.contains("Stderr:\n"));
        free_rust_string(result_ptr);
    }

    #[test]
    fn test_execute_command_echo() {
        let command = CString::new("echo hello").unwrap();
        let result_ptr = execute_command(command.as_ptr());
        let result = unsafe { CStr::from_ptr(result_ptr).to_string_lossy().into_owned() };
        let expected = "Stdout:\nhello\n\nStderr:\n";
        assert_eq!(result, expected);
        free_rust_string(result_ptr);
    }

    #[test]
    fn test_execute_command_invalid() {
        let command = CString::new("invalidcommand").unwrap();
        let result_ptr = execute_command(command.as_ptr());
        let result = unsafe { CStr::from_ptr(result_ptr).to_string_lossy().into_owned() };
        assert!(result.contains("Error executing command"));
        free_rust_string(result_ptr);
    }
}
