# zxing-cpp

zxing-cpp is a Rust wrapper for the C++ library [zxing-cpp](https://github.com/zxing-cpp/zxing-cpp).

It is an open-source, multi-format linear/matrix barcode image processing library implemented in C++. It was originally ported from the Java ZXing Library but has been developed further and now includes many improvements in terms of runtime and detection performance.

## Usage

In your Cargo.toml:

```toml
[dependencies]
# `bundled` causes cargo to compile and statically link in an up to date
# version of the c++ core library. This is the most convient and save
# way to build the library.
zxing-cpp = { version = "0.1.0", features = ["bundled", "image"] }
```

Simple example usage:

```rust
use zxing_cpp::{ImageView, ReaderOptions, BarcodeFormat, read_barcodes};

fn main() -> anyhow::Result<()> {
	let image = image::open("some-image-file.jpg")?.into_luma8();
	let iv = ImageView::from(&image);
	let opts = ReaderOptions::new()
		.formats(BarcodeFormat::QRCode | BarcodeFormat::LinearCodes)
		.try_invert(false);

	let results = read_barcodes(&iv, &opts)?;

	if results.is_empty() {
		println!("No barcode found.");
	} else {
		for result in results {
			println!("{}: {}", result.format(), result.text());
		}
	}

	Ok(())
}
```

## Optional Features

zxing-cpp provides several features that are behind [Cargo features](https://doc.rust-lang.org/cargo/reference/manifest.html#the-features-section). They are:

* `bundled` uses a bundled version of the [zxing-cpp](https://github.com/zxing-cpp/zxing-cpp) c++ library.
* [`image`](https://crates.io/crates/image) allows convenient `ImageView::from(&GreyImage)` conversion.